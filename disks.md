# Disks — CHS addressing

Old disks are addressed by three coordinates: **Head**, **Cylinder**, **Sector** (CHS).
Give the BIOS all three and it can find any block on the disk.

# Head / Surface

A disk is made of one or more platters, and data can be written on both surfaces of each
platter. The head is the index of the surface you want. It is 0-indexed.

A floppy has one platter, so only 2 surfaces -> head 0 or 1. A hard disk stacks several
platters, so it has many more (CHS allows 0 to 255). Head 0 is "the first surface" by
convention only — don't think of it as top or bottom, it's just an opaque surface index.

Set with the 'dh' register i.e. mov dh, n

# Track

Each surface is divided into concentric circles called tracks, decreasing in size as we
move from the outer edge towards the center. Indexed from 0, where 0 is the outermost.

# Cylinder

A cylinder is the set of all tracks at the same radius, across every surface. So track 5
of head 0 and track 5 of head 1 together form cylinder 5.

This is why the cylinder is a separate coordinate from the head: pick the cylinder to
choose the radius, then pick the head to choose which surface at that radius. Reading
sequentially walks through all heads of one cylinder before moving to the next cylinder,
because that needs no physical head movement.

Set with 'ch' (plus 2 more bits in 'cl' — see below).

# Sectors

Each *track* is divided into sectors. A sector is the smallest unit you can read or write,
almost always 512 bytes (same size as the boot sector). Sectors are indexed from **1**, not
0 — the odd one out among the three coordinates.

Set with the low 6 bits of 'cl'.

You can also only transfer a limited number of sectors per call, so you must also give the
count in 'al' i.e. mov al, m

# How C, H and S are packed into registers

The cylinder is 10 bits and does not fit in one register, so it is split:

    ch      = low 8 bits of the cylinder
    cl[7:6] = high 2 bits of the cylinder
    cl[5:0] = sector number (1 - 63)
    dh      = head

So `mov cl, sector` is only safe while the cylinder is <= 255. Past that you have to OR the
top cylinder bits into cl.

Resulting limits: 1024 cylinders (0-1023), 256 heads (0-255), 63 sectors (1-63).

# Example: 1.44 MB floppy

    2 heads      (0 - 1)
    80 cylinders (0 - 79)
    18 sectors   (1 - 18)
    512 bytes per sector

Every value fits comfortably inside its field, and since 79 < 256 the high cylinder bits in
cl are always zero. That is why plain `mov ch, cyl` / `mov cl, sector` appears to work on a
floppy — the packing is still there, it just never shows.

# Making the call: int 0x13, AH = 0x02 (read sectors)

Inputs:

    ah    = 0x02        function: read sectors (0x03 = write)
    al    = sector count
    ch/cl = cylinder + sector (packed as above)
    dh    = head
    dl    = drive number
    es:bx = destination buffer in memory

Outputs:

    carry clear = success, carry set = error
    ah          = error code on failure
    al          = number of sectors ACTUALLY transferred

# Gotchas

- **Don't hardcode dl.** The BIOS puts the drive you booted from in dl when it jumps to
  0x7C00. Save it immediately and pass it back. Floppy = 0x00, first hard disk = 0x80.
- **Always check the carry flag**, and compare al against the count you asked for. A
  partial read reports success-with-fewer-sectors, not an error.
- Set up es before bx — the buffer address is a full segment:offset pair.
- A single transfer must not cross a 64 KB physical boundary (legacy DMA restriction), and
  on real hardware crossing a track boundary in one call is unreliable. Keep reads small
  and loop.
- Retry on failure (reset the drive with ah = 0x00 first). Real floppy hardware genuinely
  fails the first read fairly often.

# Note

CHS is a legacy interface — the geometry it describes stopped matching real hardware
decades ago, and its limits cap out around 8 GB. Modern code uses LBA (a flat 0-indexed
block number) via the int 0x13 extensions, ah = 0x42. CHS is still worth knowing because
it is what a BIOS bootloader starts out with.