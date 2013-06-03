/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <zlib/zlib.h>
#include "WorldPacket.h"

//! Compresses packet in place
void WorldPacket::Compress(z_stream* compressionStream)
{
    uint16 uncompressedOpcode = GetOpcode();
    if (uncompressedOpcode & COMPRESSED_OPCODE_MASK)
    {
        return;
    }

    uint16 opcode = uncompressedOpcode | COMPRESSED_OPCODE_MASK;
    uint32 size = wpos();
    uint32 destsize = compressBound(size);

    std::vector<uint8> storage(destsize);

    _compressionStream = compressionStream;
    Compress(static_cast<void*>(&storage[0]), &destsize, static_cast<const void*>(contents()), size);
    if (destsize == 0)
        return;

    clear();
    reserve(destsize + sizeof(uint32));
    *this << uint32(size);
    append(&storage[0], destsize);
    SetOpcode(Opcodes(opcode));
}

//! Compresses another packet and stores it in self (source left intact)
void WorldPacket::Compress(z_stream* compressionStream, WorldPacket const* source)
{
    MANGOS_ASSERT(source != this);

    uint16 uncompressedOpcode = source->GetOpcode();
    if (uncompressedOpcode & COMPRESSED_OPCODE_MASK)
        return;

    uint16 opcode = uncompressedOpcode | COMPRESSED_OPCODE_MASK;
    uint32 size = source->size();
    uint32 destsize = compressBound(size);

    size_t sizePos = 0;
    resize(destsize + sizeof(uint32));

    _compressionStream = compressionStream;
    Compress(static_cast<void*>(&_storage[0] + sizeof(uint32)), &destsize, static_cast<const void*>(source->contents()), size);
    if (destsize == 0)
        return;

    put<uint32>(sizePos, size);
    resize(destsize + sizeof(uint32));

    SetOpcode(Opcodes(opcode));
}

void WorldPacket::Compress(void* dst, uint32 *dst_size, const void* src, int src_size)
{
    _compressionStream->next_out = (Bytef*)dst;
    _compressionStream->avail_out = *dst_size;
    _compressionStream->next_in = (Bytef*)src;
    _compressionStream->avail_in = (uInt)src_size;

    int32 z_res = deflate(_compressionStream, Z_SYNC_FLUSH);
    if (z_res != Z_OK)
    {
        *dst_size = 0;
        return;
    }

    if (_compressionStream->avail_in != 0)
    {
        *dst_size = 0;
        return;
    }

    *dst_size -= _compressionStream->avail_out;
}
