ALTER TABLE db_version CHANGE COLUMN required_11602_01_mangos_spell_proc_event required_11606_01_mangos_spell_proc_event bit;

DELETE FROM `spell_proc_event` WHERE `entry` IN (28761);
INSERT INTO `spell_proc_event` VALUES
(28761, 0x7F, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0.000000, 0.000000, 0);
