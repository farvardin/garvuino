require_relative 'constants'

$file = File.open("filter-table.h", "w")

$file.printf("#pragma once\n\n")

OCTAVES = 5

def generate_filter_lpf_table(name, q)
  $file.printf("const uint8_t g_filter_lpf_table_%s[] PROGMEM = {\n  ", name)
  (0..DATA_BYTE_MAX).each do |i|
    f = [[0, i - 4].max, 120].min
    f_0_over_fs = (2.0 ** (f / (120.0 / OCTAVES))) * (A4_PITCH * 16.0 / (SAMPLING_RATE / 2.0)) /
                  (2.0 ** (OCTAVES.to_f + 1.0))

    w_0 = 2.0 * Math::PI * f_0_over_fs
    alpha = Math.sin(w_0) / (2.0 * q)

    b_2 = (1.0 - Math.cos(w_0)) / 2.0
    a_0 = 1.0 + alpha
    a_1 = (-2.0) * Math.cos(w_0)

    b_2_over_a_0 = ((b_2 / a_0) * (1 << FILTER_TABLE_FRACTION_BITS)).floor.to_i
    b_2_over_a_0 += 0x10000 if b_2_over_a_0 < 0
    b_2_over_a_0_low = b_2_over_a_0 & 0xFF
    b_2_over_a_0_high = b_2_over_a_0 >> 8
    a_1_over_a_0 = ((a_1 / a_0) * (1 << FILTER_TABLE_FRACTION_BITS)).floor.to_i
    a_1_over_a_0 += 0x10000 if a_1_over_a_0 < 0
    a_1_over_a_0_low = a_1_over_a_0 & 0xFF
    a_1_over_a_0_high = a_1_over_a_0 >> 8

    $file.printf("0x%02x, 0x%02x, 0x%02x, 0x%02x,", b_2_over_a_0_low, b_2_over_a_0_high,
                                                    a_1_over_a_0_low, a_1_over_a_0_high)
    if i == DATA_BYTE_MAX
      $file.printf("\n")
    elsif i % 4 == (4 - 1)
      $file.printf("\n  ")
    else
      $file.printf(" ")
    end
  end
  $file.printf("};\n\n")
end

(0..8).each do |idx|
  generate_filter_lpf_table(idx.to_s, Math.sqrt(2.0) ** (idx - 1.0))
end

$file.printf("const uint8_t* g_filter_lpf_tables[] = {\n  ")
(0..8).each do |idx|
  $file.printf("g_filter_lpf_table_%-2d,", idx)
  if idx == DATA_BYTE_MAX
    $file.printf("\n")
  elsif idx % 4 == 3
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")


$file.close
