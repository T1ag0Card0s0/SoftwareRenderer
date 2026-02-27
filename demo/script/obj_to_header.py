import sys
import os

def parse_face_vertex(token: str) -> int:
    """
    Returns OBJ position index (1-based in file; may be negative).
    token examples: '3', '3/1', '3/1/9', '3//9'
    """
    if '/' in token:
        token = token.split('/')[0]
    return int(token)

def obj_to_define_header(obj_filename):
    positions = []  # list of (x,y,z)
    triangles = []  # list of (i0,i1,i2) indices into positions (0-based)

    with open(obj_filename, 'r') as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith('#'):
                continue

            if line.startswith('v '):
                parts = line.split()
                x, y, z = float(parts[1]), float(parts[2]), float(parts[3])
                positions.append((x, y, z))

            elif line.startswith('f '):
                parts = line.split()[1:]
                if len(parts) < 3:
                    continue

                # Convert OBJ indices to 0-based indices into positions[]
                face = []
                for tok in parts:
                    idx = parse_face_vertex(tok)
                    # OBJ allows negative indices: -1 means last defined vertex
                    if idx < 0:
                        idx = len(positions) + idx + 1
                    face.append(idx - 1)

                # Triangulate as a fan: (0, i, i+1)
                v0 = face[0]
                for i in range(1, len(face) - 1):
                    triangles.append((v0, face[i], face[i + 1]))

    # Build flat float array for positions
    vertex_floats = []
    for (x, y, z) in positions:
        vertex_floats.extend([x, y, z])

    # Build flat index array (uint32)
    indices = []
    for (a, b, c) in triangles:
        indices.extend([a, b, c])

    header_filename = os.path.splitext(obj_filename)[0] + ".h"
    base_name = os.path.splitext(os.path.basename(obj_filename))[0]
    macro = base_name.upper()

    float_count = len(vertex_floats)
    index_count = len(indices)
    vertex_count = len(positions)
    triangle_count = len(triangles)

    with open(header_filename, 'w') as f:
        f.write(f"#ifndef {macro}_H\n")
        f.write(f"#define {macro}_H\n\n")
        f.write(f"#include <stdint.h>\n\n")

        f.write(f"#define {macro}_VERTEX_COUNT {vertex_count}\n")
        f.write(f"#define {macro}_FLOATS {float_count}\n")
        f.write(f"#define {macro}_TRIANGLE_COUNT {triangle_count}\n")
        f.write(f"#define {macro}_INDEX_COUNT {index_count}\n\n")

        f.write(f"#define {macro}_VERTICES \\\n")
        for i in range(0, float_count, 3):
            f.write(f"    {vertex_floats[i]:.6f}f, {vertex_floats[i+1]:.6f}f, {vertex_floats[i+2]:.6f}f,")
            if i + 3 < float_count:
                f.write(" \\\n")
            else:
                f.write("\n")

        f.write("\n")
        f.write(f"#define {macro}_INDICES \\\n")
        for i in range(0, index_count, 3):
            f.write(f"    {indices[i]}u, {indices[i+1]}u, {indices[i+2]}u,")
            if i + 3 < index_count:
                f.write(" \\\n")
            else:
                f.write("\n")

        f.write(f"\n#endif // {macro}_H\n")

    print(f"Header file generated: {header_filename}")
    print(f"  vertices: {vertex_count} ({float_count} floats)")
    print(f"  triangles: {triangle_count} ({index_count} indices)")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python obj_to_header.py model.obj")
        sys.exit(1)

    obj_to_define_header(sys.argv[1])
