require_relative 'DigitalSynthVRA8Px/constants'

$file = File.open("sample-midi-stream.bin", "wb")

def control_change(control_number, value)
  $file.write([(CONTROL_CHANGE | MIDI_CH), control_number, value].pack("C*"))
end

def note_on(note_number, velocity)
  $file.write([(NOTE_ON  | MIDI_CH), note_number, velocity].pack("C*"))
end

def note_off(note_number)
  $file.write([(NOTE_OFF | MIDI_CH), note_number, 64].pack("C*"))
end

def wait(length)
  length.times { $file.write([ACTIVE_SENSING].pack("C")) }
end

def play_a(oct)
  play_triad_a(12, 16, 19, oct, 64)
  play_triad_a(16, 19, 23, oct, 32)
  play_triad_a(14, 17, 21, oct, 64)
  play_triad_a(17, 21, 24, oct, 96)
end

def play_b(oct)
  play_triad_b(12, 16, 19, oct, 64)
  play_triad_b(16, 19, 23, oct, 32)
  play_triad_b(14, 17, 21, oct, 64)
  play_triad_b(17, 21, 24, oct, 96)
end

def play_triad_a(x, y, z, oct, velocity)
  note_on(x + (oct * 12), velocity)
  note_on(y + (oct * 12), velocity)
  note_on(z + (oct * 12), velocity)
  wait(5000)
  note_off(x + (oct * 12))
  note_off(y + (oct * 12))
  note_off(z + (oct * 12))
  wait(1250)
end

def play_triad_b(x, y, z, oct, velocity)
  note_on(x + (oct * 12), velocity)
  wait(1250)
  note_on(y + (oct * 12), velocity)
  wait(1250)
  note_on(z + (oct * 12), velocity)
  wait(5000)
  note_off(z + (oct * 12))
  wait(2500)
  note_off(y + (oct * 12))
  wait(2500)
  note_off(x + (oct * 12))
  wait(1250)
end

def sound_off
  control_change(ALL_NOTES_OFF, 0  )
  wait(1250)
end

sound_off

control_change(UNISON       , 0  )
control_change(OSC_WAVEFORM , 0  )
control_change(OSC_DETUNE   , 48 )
control_change(FILTER_NG_AMT, 64 )

control_change(FILTER_CUTOFF, 64 )
control_change(FILTER_RESO  , 64 )
control_change(FILTER_EG_AMT, 96 )
control_change(EG_DECAY     , 64 )

control_change(EG_ATTACK    , 0  )
control_change(DETUNE_NG_AMT, 32 )
control_change(DETUNE_EG_AMT, 48 )
control_change(AMP_EG       , 0  )

control_change(VELOCITY_SENS, 0  )
control_change(CUTOFF_V_SENS, 127)
control_change(PORTAMENTO   , 32 )
control_change(UNISON_OPTION, 0  )

play_a(4)
play_b(3)

sound_off

$file.close
