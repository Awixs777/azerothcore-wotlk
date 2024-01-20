-- respawn creatures that were deleted due to guid conflicts
-- entries taken from updates/db_world/2023_12_12_07.sql
DELETE FROM `creature` WHERE (`guid` BETWEEN 12635 AND 12648);
INSERT INTO `creature` (`guid`, `id1`, `map`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(12635, 26258, 0, 1, 1, 0, -8247.5283203125, -2623.9853515625, 133.23797607421875, 0, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12636, 26258, 0, 1, 1, 0, 999.045166015625, -1453.84375, 60.8197021484375, 3.682644605636596679, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12637, 26258, 1, 1, 1, 0, -4399.1708984375, 3486.65185546875, 12.37061214447021484, 0, 120, 0,  0, 0, 0, 0, "", 50172, 1, NULL),
(12638, 26258, 530, 1, 1, 0, -2247.220703125, -11898.091796875, 27.01277351379394531, 1.675516128540039062, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12639, 26258, 530, 1, 1, 0, -2526.489501953125, 7551.50634765625, -2.26869177818298339, 2.146754980087280273, 120, 0,  0, 0, 0, 0, "", 50172, 1, NULL),
(12640, 26258, 530, 1, 1, 0, -2999.47412109375, 4155.458984375, 4.710585594177246093, 4.939281940460205078, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12641, 26258, 530, 1, 1, 0, -3792.8955078125, -11502.70703125, -134.6624755859375, 0, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12642, 26258, 530, 1, 1, 0, -3943.538330078125, 2049.31396484375, 95.148040771484375, 6.091198921203613281, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12643, 26258, 530, 1, 1, 0, -4223.84326171875, -12318.365234375, 2.560283184051513671, 2.932153224945068359, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12644, 26258, 530, 1, 1, 0, 187.83258056640625, 6024.68896484375, 21.64208793640136718, 2.967059612274169921, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12645, 26258, 530, 1, 1, 0, 2019.80322265625, 6580.18603515625, 134.4452056884765625, 1.93731546401977539, 120, 0,  0, 0, 0, 0, "", 50172, 1, NULL),
(12646, 26258, 530, 1, 1, 0, 3121.99560546875, 3751.6884765625, 141.7324676513671875, 0, 120, 0,  0, 0, 0, 0, "", 50063, 1, NULL),
(12647, 26258, 571, 1, 1, 0, 2481.042236328125, -4891.05712890625, 265.143035888671875, 4.485496044158935546, 120, 0,  0, 0, 0, 0, "", 50172, 1, NULL),
(12648, 26258, 571, 1, 1, 0, 3395.283203125, -2904.61962890625, 202.542633056640625, 3.560471534729003906, 120, 0,  0, 0, 0, 0, "", 50172, 1, NULL);
