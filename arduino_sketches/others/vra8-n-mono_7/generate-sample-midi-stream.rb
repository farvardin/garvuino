require_relative 'DigitalSynthVRA8N/constants'

$file = File.open("sample-midi-stream.bin", "wb")

def control_change(control_number, value)
  $file.write([(CONTROL_CHANGE | MIDI_CH), control_number, value].pack("C*"))
  wait(10)
end

def program_change(program_number)
  $file.write([(PROGRAM_CHANGE | MIDI_CH), program_number].pack("C*"))
  wait(800)
end

def play(note_number, length)
  $file.write([(NOTE_ON  | MIDI_CH), note_number, 64].pack("C*"))
  (length * 15 / 16).times { $file.write([ACTIVE_SENSING].pack("C")) }
  $file.write([(NOTE_OFF | MIDI_CH), note_number, 64].pack("C*"))
  (length * 1 / 16).times { $file.write([ACTIVE_SENSING].pack("C")) }
end

def wait(length)
  length.times { $file.write([ACTIVE_SENSING].pack("C")) }
end

def play_cegbdfac(c)
  play(12 + (c * 12), 1200)
  play(19 + (c * 12), 1200)
  play(16 + (c * 12), 1200)
  play(23 + (c * 12), 1200)
  play(14 + (c * 12), 1200)
  play(21 + (c * 12), 1200)
  play(17 + (c * 12), 1200)
  play(24 + (c * 12), 8400)
  wait(8400)
end

def sound_off
  control_change(ALL_NOTES_OFF, 0  )
  wait(800)
end

sound_off

program_change(0)
play_cegbdfac(3)
sound_off

program_change(1)
play_cegbdfac(3)
sound_off

program_change(2)
play_cegbdfac(2)
sound_off

program_change(3)
play_cegbdfac(4)
sound_off

program_change(7)
play_cegbdfac(4)
sound_off

$file.close
