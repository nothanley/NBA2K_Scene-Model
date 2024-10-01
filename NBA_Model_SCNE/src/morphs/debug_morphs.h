#include <vector>
#include <string>
#pragma once


// DEBUG MORPH LIST - HACKY, MOVE THIS ...
static std::vector<std::string> debugMorphs = {
"brow_innerDown_L",
"brow_innerDown_R",
"brow_innerRaise_L",
"brow_innerRaise_R",
"brow_lateral_L",
"brow_lateral_R",
"brow_outerDown_L",
"brow_outerDown_R",
"brow_outerRaise_L",
"brow_outerRaise_R",
"cheek_bInnerDown_bLateral_bOuterDown_cSquint_eBlink_DLT_L",
"cheek_bInnerDown_bLateral_bOuterDown_cSquint_eBlink_DLT_R",
"cheek_bInnerDown_bOuterDown_cSquint_eBlink_DLT_L",
"cheek_bInnerDown_bOuterDown_cSquint_eBlink_DLT_R",
"cheek_bInnerDown_bLateral_bOuterDown_cSquint_DLT_L",
"cheek_bInnerDown_bLateral_bOuterDown_cSquint_DLT_R",
"cheek_bInnerDown_bOuterDown_cSquint_DLT_L",
"cheek_bInnerDown_bOuterDown_cSquint_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_lCornersWide_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_lCornersWide_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_lCornersWide_nWrinkler_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_lCornersWide_nWrinkler_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_nWrinkler_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_lCornersUp_nWrinkler_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_lCornersWide_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_lCornersWide_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_lCornersWide_nWrinkler_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_lCornersWide_nWrinkler_DLT_R",
"cheek_bOuterDown_cSquint_eBlink_nWrinkler_DLT_L",
"cheek_bOuterDown_cSquint_eBlink_nWrinkler_DLT_R",
"cheek_bOuterDown_cSquint_DLT_L",
"cheek_bOuterDown_cSquint_DLT_R",
"cheek_bOuterDown_eBlink_DLT_L",
"cheek_bOuterDown_eBlink_DLT_R",
"cheek_cSquint_eBlink_DLT_L",
"cheek_cSquint_eBlink_DLT_R",
"cheek_cSquint_lCornersUp_nWrinkler_DLT_L",
"cheek_cSquint_lCornersUp_nWrinkler_DLT_R",
"cheek_cSquint_nWrinkler_DLT_L",
"cheek_cSquint_nWrinkler_DLT_R",
"cheek_squint_L",
"cheek_squint_R",
"cheek_suck_L",
"cheek_suck_R",
"cheek_blow_L",
"cheek_blow_R",
"chin_raise_B",
"chin_raise_T",
"eyelid_bInnerRaise_bOuterRaise_eBlink_DLT_L",
"eyelid_bInnerRaise_bOuterRaise_eBlink_DLT_R",
"eyelid_bInnerRaise_bOuterRaise_eLookDown_DLT_L",
"eyelid_bInnerRaise_bOuterRaise_eLookDown_DLT_R",
"eyelid_bInnerRaise_eBlink_DLT_L",
"eyelid_bInnerRaise_eBlink_DLT_R",
"eyelid_bInnerRaise_eLookDown_DLT_L",
"eyelid_bInnerRaise_eLookDown_DLT_R",
"eyelid_blink_DLT_L",
"eyelid_blink_DLT_R",
"eyelid_bOuterRaise_eBlink_DLT_L",
"eyelid_bOuterRaise_eBlink_DLT_R",
"eyelid_bOuterRaise_eLookDown_DLT_L",
"eyelid_bOuterRaise_eLookDown_DLT_R",
"eyelid_eBlink_eLookDown_DLT_L",
"eyelid_eBlink_eLookDown_DLT_R",
"eyelid_eBlink_eLookUp_DLT_L",
"eyelid_eBlink_eLookUp_DLT_R",
"eyelid_halfLookLeft_DLT_L",
"eyelid_halfLookLeft_DLT_R",
"eyelid_halfLookRight_DLT_L",
"eyelid_halfLookRight_DLT_R",
"eyelid_halfBlink_DLT_L",
"eyelid_halfBlink_DLT_R",
"eyelid_halfBlink_L",
"eyelid_halfBlink_R",
"eyelid_eLowerDroop_eBlink_DLT_L",
"eyelid_eLowerDroop_eBlink_DLT_R",
"eyelid_lookDown_L",
"eyelid_lookDown_R",
"eyelid_lookLeft_DLT_L",
"eyelid_lookLeft_DLT_R",
"eyelid_lookRight_DLT_L",
"eyelid_lookRight_DLT_R",
"eyelid_lookUp_L",
"eyelid_lookUp_R",
"eyelid_lowerDroop_L",
"eyelid_lowerDroop_R",
"eyelid_upperRaise_L",
"eyelid_upperRaise_R",
"eyelid_eBlink_nWrinkler_DLT_L",
"eyelid_eBlink_nWrinkler_DLT_R",
"eyelid_glare_L",
"eyelid_glare_R",
"eyelid_hardSqueeze_L",
"eyelid_hardSqueeze_R",
"eyelid_squeeze_L",
"eyelid_squeeze_R",
"eyelid_eGlare_eBlink_DLT_L",
"eyelid_eGlare_eBlink_DLT_R",
"jaw_back_C",
"jaw_forward_C",
"jaw_left_C",
"jaw_openExtreme_DLT_C",
"jaw_open_C",
"jaw_right_C",
"lips_blow_L",
"lips_blow_R",
"lips_bottomLeft_C",
"lips_bottomRight_C",
"lips_bottomRotateIn_C",
"lips_bottomRotateOut_C",
"lips_converge_LIB",
"lips_converge_LIT",
"lips_converge_LOB",
"lips_converge_LOT",
"lips_converge_RIB",
"lips_converge_RIT",
"lips_converge_ROB",
"lips_converge_ROT",
"lips_cornersDown_L",
"lips_cornersDown_R",
"lips_cornersRound_LB",
"lips_cornersRound_LT",
"lips_cornersRound_RB",
"lips_cornersRound_RT",
"lips_cornersSharp_LB",
"lips_cornersSharp_LT",
"lips_cornersSharp_RB",
"lips_cornersSharp_RT",
"lips_cornersUp_L",
"lips_cornersUp_R",
"lips_diverge_LIB",
"lips_diverge_LIT",
"lips_diverge_LOB",
"lips_diverge_LOT",
"lips_diverge_RIB",
"lips_diverge_RIT",
"lips_diverge_ROB",
"lips_diverge_ROT",
"lips_jOpen_lConverge_lPucker_DLT_LIB",
"lips_jOpen_lConverge_lPucker_DLT_LIT",
"lips_jOpen_lConverge_lPucker_DLT_LOB",
"lips_jOpen_lConverge_lPucker_DLT_LOT",
"lips_jOpen_lConverge_lPucker_DLT_RIB",
"lips_jOpen_lConverge_lPucker_DLT_RIT",
"lips_jOpen_lConverge_lPucker_DLT_ROB",
"lips_jOpen_lConverge_lPucker_DLT_ROT",
"lips_jOpen_lCornersDown_DLT_L",
"lips_jOpen_lCornersDown_DLT_R",
"lips_jOpen_lCornersDown_lDiverge_DLT_LIB",
"lips_jOpen_lCornersDown_lDiverge_DLT_LIT",
"lips_jOpen_lCornersDown_lDiverge_DLT_LOB",
"lips_jOpen_lCornersDown_lDiverge_DLT_LOT",
"lips_jOpen_lCornersDown_lDiverge_DLT_RIB",
"lips_jOpen_lCornersDown_lDiverge_DLT_RIT",
"lips_jOpen_lCornersDown_lDiverge_DLT_ROB",
"lips_jOpen_lCornersDown_lDiverge_DLT_ROT",
"lips_jOpen_lCornersUp_DLT_L",
"lips_jOpen_lCornersUp_DLT_R",
"lips_jOpen_lCornersUp_lDiverge_DLT_LIB",
"lips_jOpen_lCornersUp_lDiverge_DLT_LIT",
"lips_jOpen_lCornersUp_lDiverge_DLT_LOB",
"lips_jOpen_lCornersUp_lDiverge_DLT_LOT",
"lips_jOpen_lCornersUp_lDiverge_DLT_RIB",
"lips_jOpen_lCornersUp_lDiverge_DLT_RIT",
"lips_jOpen_lCornersUp_lDiverge_DLT_ROB",
"lips_jOpen_lCornersUp_lDiverge_DLT_ROT",
"lips_jOpen_lCornersUp_lDiverge_nWrinkler_DLT_LIT",
"lips_jOpen_lCornersUp_lDiverge_nWrinkler_DLT_LOT",
"lips_jOpen_lCornersUp_lDiverge_nWrinkler_DLT_RIT",
"lips_jOpen_lCornersUp_lDiverge_nWrinkler_DLT_ROT",
"lips_jOpen_lCornersUp_nWrinkler_DLT_L",
"lips_jOpen_lCornersUp_nWrinkler_DLT_R",
"lips_jOpen_lDiverge_lPucker_DLT_LIB",
"lips_jOpen_lDiverge_lPucker_DLT_LIT",
"lips_jOpen_lDiverge_lPucker_DLT_LOB",
"lips_jOpen_lDiverge_lPucker_DLT_LOT",
"lips_jOpen_lDiverge_lPucker_DLT_RIB",
"lips_jOpen_lDiverge_lPucker_DLT_RIT",
"lips_jOpen_lDiverge_lPucker_DLT_ROB",
"lips_jOpen_lDiverge_lPucker_DLT_ROT",
"lips_jOpen_lPucker_DLT_L",
"lips_jOpen_lPucker_DLT_R",
"lips_jOpen_lDiverge_DLT_LIB",
"lips_jOpen_lDiverge_DLT_LIT",
"lips_jOpen_lDiverge_DLT_LOB",
"lips_jOpen_lDiverge_DLT_LOT",
"lips_jOpen_lDiverge_DLT_RIB",
"lips_jOpen_lDiverge_DLT_RIT",
"lips_jOpen_lDiverge_DLT_ROB",
"lips_jOpen_lDiverge_DLT_ROT",
"lips_cSquint_lCornersUp_DLT_L",
"lips_cSquint_lCornersUp_DLT_R",
"lips_cSquint_lCornersUp_lCornersWide_DLT_L",
"lips_cSquint_lCornersUp_lCornersWide_DLT_R",
"lips_cSquint_lCornersUp_lCornersWide_nWrinkler_DLT_L",
"lips_cSquint_lCornersUp_lCornersWide_nWrinkler_DLT_R",
"lips_cSquint_lCornersWide_DLT_L",
"lips_cSquint_lCornersWide_DLT_R",
"lips_lConvergeBottom_lDivergeTop_DLT_LIB",
"lips_lConvergeBottom_lDivergeTop_DLT_LOB",
"lips_lConvergeBottom_lDivergeTop_DLT_RIB",
"lips_lConvergeBottom_lDivergeTop_DLT_ROB",
"lips_lConvergeTop_lDivergeBottom_DLT_LIT",
"lips_lConvergeTop_lDivergeBottom_DLT_LOT",
"lips_lConvergeTop_lDivergeBottom_DLT_RIT",
"lips_lConvergeTop_lDivergeBottom_DLT_ROT",
"lips_lCornersWide_DLT_L",
"lips_lCornersWide_DLT_R",
"lips_lCornersDown_lDiverge_DLT_LIB",
"lips_lCornersDown_lDiverge_DLT_LIT",
"lips_lCornersDown_lDiverge_DLT_LOB",
"lips_lCornersDown_lDiverge_DLT_LOT",
"lips_lCornersDown_lDiverge_DLT_RIB",
"lips_lCornersDown_lDiverge_DLT_RIT",
"lips_lCornersDown_lDiverge_DLT_ROB",
"lips_lCornersDown_lDiverge_DLT_ROT",
"lips_lCornersUp_lCornersWide_nWrinkler_DLT_L",
"lips_lCornersUp_lCornersWide_nWrinkler_DLT_R",
"lips_lCornersUp_lDiverge_DLT_LIB",
"lips_lCornersUp_lDiverge_DLT_LIT",
"lips_lCornersUp_lDiverge_DLT_LOB",
"lips_lCornersUp_lDiverge_DLT_LOT",
"lips_lCornersUp_lDiverge_DLT_RIB",
"lips_lCornersUp_lDiverge_DLT_RIT",
"lips_lCornersUp_lDiverge_DLT_ROB",
"lips_lCornersUp_lDiverge_DLT_ROT",
"lips_lCornersUp_lDiverge_nWrinkler_DLT_LIT",
"lips_lCornersUp_lDiverge_nWrinkler_DLT_LOT",
"lips_lCornersUp_lDiverge_nWrinkler_DLT_RIT",
"lips_lCornersUp_lDiverge_nWrinkler_DLT_ROT",
"lips_lCornersUp_nWrinkler_DLT_L",
"lips_lCornersUp_nWrinkler_DLT_R",
"lips_lCornersWide_nWrinkler_DLT_L",
"lips_lCornersWide_nWrinkler_DLT_R",
"lips_lDiverge_lPucker_DLT_LIB",
"lips_lDiverge_lPucker_DLT_LIT",
"lips_lDiverge_lPucker_DLT_LOB",
"lips_lDiverge_lPucker_DLT_LOT",
"lips_lDiverge_lPucker_DLT_RIB",
"lips_lDiverge_lPucker_DLT_RIT",
"lips_lDiverge_lPucker_DLT_ROB",
"lips_lDiverge_lPucker_DLT_ROT",
"lips_lPress_lPucker_DLT_LB",
"lips_lPress_lPucker_DLT_LT",
"lips_lPress_lPucker_DLT_RB",
"lips_lPress_lPucker_DLT_RT",
"lips_lPucker_mLeft_DLT_L",
"lips_lPucker_mLeft_DLT_R",
"lips_lPucker_mRight_DLT_L",
"lips_lPucker_mRight_DLT_R",
"lips_lPucker_nWrinkler_DLT_L",
"lips_lPucker_nWrinkler_DLT_R",
"lips_pressBottom_L",
"lips_pressBottom_R",
"lips_pressTop_L",
"lips_pressTop_R",
"lips_pucker_L",
"lips_pucker_R",
"lips_pull_B",
"lips_pull_T",
"lips_push_B",
"lips_push_T",
"lips_sticky_PH1_L",
"lips_sticky_PH1_R",
"lips_sticky_PH2_L",
"lips_sticky_PH2_R",
"lips_sticky_PH3_L",
"lips_sticky_PH3_R",
"lips_sticky_PH4_L",
"lips_sticky_PH4_R",
"lips_sticky_PH5_L",
"lips_sticky_PH5_R",
"lips_topLeft_C",
"lips_topRight_C",
"lips_topRotateIn_C",
"lips_topRotateOut_C",
"lips_tweakCornersDown_L",
"lips_tweakCornersDown_R",
"lips_tweakCornersIn_L",
"lips_tweakCornersIn_R",
"lips_tweakCornersOut_L",
"lips_tweakCornersOut_R",
"lips_tweakCornersUp_L",
"lips_tweakCornersUp_R",
"lips_wideDown_L",
"lips_wideDown_R",
"lips_wideUp_L",
"lips_wideUp_R",
"lips_bottomCurl_C",
"mouth_down_C",
"mouth_left_C",
"mouth_right_C",
"mouth_up_C",
"neck_swallow_ph1",
"neck_swallow_ph2",
"neck_swallow_ph3",
"neck_swallow_ph4",
"nose_compress_L",
"nose_compress_R",
"nose_depress_L",
"nose_depress_R",
"nose_flare_L",
"nose_flare_R",
"nose_furrow_L",
"nose_furrow_R",
"nose_wrinkler_L",
"nose_wrinkler_R",
"cheek_cSquint_eBlink_nWrinkler_DLT_L",
"cheek_cSquint_eBlink_nWrinkler_DLT_R",
"narrow",
"widen",
"thick",
"thin",
"l_elbow_region",
"l_arm_forwardBack_region",
"l_leg_region",
"l_knee_region",
"torso_twist_region",
"r_elbow_region",
"r_arm_forwardBack_region",
"r_leg_region",
"r_knee_region",
"l_armBack",
"l_armDown",
"l_armForward",
"l_armUpExtreme",
"r_armBack",
"r_armDown",
"r_armForward",
"r_armUpExtreme",
"torsoBend_fwd",
"torsoBend_back",
"torsoBend_l",
"torsoBend_r",
"torsoTwist_l",
"torsoTwist_r",
"l_armDown_THIN",
"r_armDown_THIN",
"l_armUpExtreme_THIN",
"r_armUpExtreme_THIN",
"l_armUpExtreme_THICK",
"r_armUpExtreme_THICK",
"l_armBack_THIN",
"r_armBack_THIN",
"l_armBack_THICK",
"r_armBack_THICK",
"l_elbowBendFull",
"r_elbowBendFull",
"l_elbowBendHalf",
"r_elbowBendHalf",
"l_elbowBendHalf_THIN",
"r_elbowBendHalf_THIN",
"l_elbowBendHalf_THICK",
"r_elbowBendHalf_THICK",
"l_elbowBendFull_THIN",
"r_elbowBendFull_THIN",
"l_elbowBendFull_THICK",
"r_elbowBendFull_THICK",
"l_wristIn",
"l_wristOut",
"l_wristUp",
"r_kneeBend",
"r_wristDown",
"r_wristIn",
"r_wristOut",
"r_wristUp",
"l_kneeBend",
"l_wristDown",
"inertiaDown",
"inertiaUp",
"neckBack",
"l_legBack",
"l_legForward",
"r_legBack",
"r_legForward",
"l_neckTurn",
"r_neckTurn",
"sit",
"chinDown",
"neckForward",
"l_legOut",
"r_legOut",
"l_anklePoint",
"r_anklePoint",
"skirt_region",
"l_armTpose",
"r_armTpose",
"l_armTpose_THIN",
"r_armTpose_THIN",
"neckBack_l",
"neckBack_r",
"l_finger_1_2_bend",
"l_finger_2_2_bend",
"l_finger_3_2_bend",
"l_finger_4_2_bend",
"r_finger_1_2_bend",
"r_finger_2_2_bend",
"r_finger_3_2_bend",
"r_finger_4_2_bend",
"forearmUpperTHICK",
"forearmLowerTHICK",
"upperarmTHICK",
"trapTHICK",
"neckTHICK",
"chestTHICK",
"breastTHICK",
"absTHICK",
"headTHICK",
"shoulderTHICK",
"hipsTHICK",
"glutesTHICK",
"thighTHICK",
"calfUpperTHICK",
"calfLowerTHICK",
"forearmUpperTHIN",
"forearmLowerTHIN",
"upperarmTHIN",
"trapTHIN",
"neckTHIN",
"chestTHIN",
"breastTHIN",
"absTHIN",
"headTHIN",
"shoulderTHIN",
"hipsTHIN",
"glutesTHIN",
"thighTHIN",
"calfUpperTHIN",
"calfLowerTHIN",
"shoulderRIPPED",
"shoulderSOFT",
"thin_legs" };