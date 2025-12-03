from PIL import Image

img = Image.open("player.ppm").convert("RGB")

print("const unsigned char player_texture[256] = {")

for y in range(16):
    row_bytes = []
    for x in range(16):
        r, g, b = img.getpixel((x, y))

        r3 = (r * 7) // 255
        g3 = (g * 7) // 255
        b2 = (b * 3) // 255

        val = ((r3 & 0b111) << 5) | ((g3 & 0b111) << 2) | (b2 & 0b11)

        row_bytes.append(f"0x{val:02X}")

    print("   " + ", ".join(row_bytes) + ",")

print("};")
