#pragma once

const uint8_t PROGRAM_NUMBER_DEFAULT = 0;

#if !defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)

// PRESET                                       #0   #1   #2   #3   #4   #5   #6   #7   
const uint8_t g_preset_table_OSC_WAVE     [] = {0  , 0  , 0  , 127, 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_OSC2_MIX     [] = {64 , 64 , 64 , 64 , 0  , 64 , 0  , 0  };
const uint8_t g_preset_table_OSC2_COARSE  [] = {71 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_OSC2_FINE    [] = {68 , 72 , 66 , 72 , 68 , 56 , 64 , 64 };

const uint8_t g_preset_table_SUB_OSC_WAVE [] = {127, 0  , 0  , 0  , 127, 0  , 0  , 0  };
const uint8_t g_preset_table_SUB_OSC_LEVEL[] = {127, 127, 127, 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_PITCH_EG_AMT [] = {64 , 62 , 64 , 64 , 64 , 72 , 64 , 64 };
const uint8_t g_preset_table_PITCH_EG_TGT [] = {0  , 127, 0  , 0  , 0  , 127, 0  , 0  };

const uint8_t g_preset_table_FILTER_CUTOFF[] = {76 , 28 , 0  , 127, 127, 118, 127, 127};
const uint8_t g_preset_table_FILTER_RESO  [] = {80 , 64 , 127, 0  , 0  , 32 , 0  , 0  };
const uint8_t g_preset_table_CUTOFF_EG_AMT[] = {82 , 112, 94,  64 , 64 , 60 , 64 , 64 };
const uint8_t g_preset_table_AMP_EG_ON    [] = {127, 0  , 127, 0  , 127, 127, 127, 127};

const uint8_t g_preset_table_EG_ATTACK    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_DECAY     [] = {64 , 96 , 96 , 96 , 0  , 32 , 0  , 0  };
const uint8_t g_preset_table_EG_SUSTAIN   [] = {127, 0  , 0  , 0  , 127, 96 , 127, 127};
const uint8_t g_preset_table_EG_RELEASE   [] = {127, 127, 127, 127, 127, 0  , 127, 127};

const uint8_t g_preset_table_LFO_WAVE     [] = {0  , 0  , 0  , 127, 0  , 32 , 0  , 0  };
const uint8_t g_preset_table_LFO_RATE     [] = {64 , 64 , 64 , 80 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_PITCH_LFO_AMT[] = {65 , 65 , 64 , 88 , 65 , 65 , 64 , 64 };
const uint8_t g_preset_table_CO_LFO_AMT   [] = {64 , 64 , 52 , 64 , 64 , 64 , 64 , 64 };

const uint8_t g_preset_table_LFO_DEPTH    [] = {16 , 16 , 0  , 0  , 0  , 16 , 0  , 0  };
const uint8_t g_preset_table_LFO_RT_EG_AMT[] = {64 , 64 , 64 , 72 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_PITCH_LFO_TGT[] = {127, 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_CC15         [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_PB_RANGE     [] = {2  , 2  , 2  , 2  , 12 , 2  , 2  , 2  };
const uint8_t g_preset_table_CC86         [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_CO_EXP_AMT   [] = {64 , 64 , 64 , 64 , 64 , 94 , 64 , 64 };
const uint8_t g_preset_table_AMP_EXP_AMT  [] = {127, 127, 127, 127, 127, 127, 127, 127};

const uint8_t g_preset_table_PORTAMENTO   [] = {0  , 32 , 32 , 0  , 0  , 32 , 0  , 0  };
const uint8_t g_preset_table_LEGATO       [] = {0  , 127, 127, 0  , 0  , 127, 0  , 0  };
const uint8_t g_preset_table_KEY_ASSIGN   [] = {0  , 64 , 0  , 127, 0  , 127, 0  , 0  };
const uint8_t g_preset_table_EXP_BY_VEL   [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

#else

// PRESET                                       #0   #1   #2   #3   #4   #5   #6   #7   
const uint8_t g_preset_table_OSC_WAVE     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_OSC2_MIX     [] = {64 , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_OSC2_COARSE  [] = {71 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_OSC2_FINE    [] = {68 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };

const uint8_t g_preset_table_SUB_OSC_WAVE [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_SUB_OSC_LEVEL[] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_PITCH_EG_AMT [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_PITCH_EG_TGT [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_FILTER_CUTOFF[] = {64 , 127, 127, 127, 127, 127, 127, 127};
const uint8_t g_preset_table_FILTER_RESO  [] = {64 , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_CUTOFF_EG_AMT[] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_AMP_EG_ON    [] = {127, 127, 127, 127, 127, 127, 127, 127};

const uint8_t g_preset_table_EG_ATTACK    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_DECAY     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_SUSTAIN   [] = {127, 127, 127, 127, 127, 127, 127, 127};
const uint8_t g_preset_table_EG_RELEASE   [] = {127, 127, 127, 127, 127, 127, 127, 127};

const uint8_t g_preset_table_LFO_WAVE     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LFO_RATE     [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_PITCH_LFO_AMT[] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_CO_LFO_AMT   [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };

const uint8_t g_preset_table_LFO_DEPTH    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LFO_RT_EG_AMT[] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_PITCH_LFO_TGT[] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_CC15         [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_PB_RANGE     [] = {30 , 2  , 2  , 2  , 2  , 2  , 2  , 2  };
const uint8_t g_preset_table_CC86         [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_CO_EXP_AMT   [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_AMP_EXP_AMT  [] = {127, 127, 127, 127, 127, 127, 127, 127};

const uint8_t g_preset_table_PORTAMENTO   [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LEGATO       [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_KEY_ASSIGN   [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EXP_BY_VEL   [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

#endif
