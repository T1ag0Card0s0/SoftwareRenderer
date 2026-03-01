import sys
import os

def obj_to_define_header(obj_filename):
    vertices = []
    indices = []

    with open(obj_filename, 'r') as f:
        for line in f:
            if line.startswith('v '):
                parts = line.strip().split()
                vertices.extend([float(parts[1]), float(parts[2]), float(parts[3])])
            elif line.startswith('f '):
                parts = line.strip().split()
                face = [int(p.split('/')[0]) - 1 for p in parts[1:]]
                if len(face) < 3:
                    print(f"Warning: degenerate face skipped: {line.strip()}")
                    continue
                for i in range(1, len(face) - 1):
                    indices.extend([face[0], face[i], face[i + 1]])

    vertex_count = len(vertices) // 3
    index_count  = len(indices)

    header_filename = os.path.splitext(obj_filename)[0] + ".h"
    base_name       = os.path.splitext(os.path.basename(obj_filename))[0]
    prefix          = base_name.upper()

    with open(header_filename, 'w') as f:
        f.write(f"#ifndef {prefix}_H\n")
        f.write(f"#define {prefix}_H\n\n")
        f.write(f"#include <stdint.h>\n\n")

        f.write(f"#define {prefix}_VERTEX_COUNT {vertex_count}\n")
        f.write(f"#define {prefix}_INDEX_COUNT  {index_count}\n\n")

        f.write(f"#define {prefix}_VERTICES \\\n")
        float_count = len(vertices)
        for i in range(0, float_count, 3):
            f.write(f"    {vertices[i]:.6f}f, {vertices[i+1]:.6f}f, {vertices[i+2]:.6f}f,")
            if i + 3 < float_count:
                f.write(" \\\n")
            else:
                f.write("\n")

        f.write("\n")

        f.write(f"#define {prefix}_INDICES \\\n")
        for i in range(0, index_count, 3):
            a, b, c = indices[i], indices[i+1], indices[i+2]
            f.write(f"    {a}u, {b}u, {c}u,")
            if i + 3 < index_count:
                f.write(" \\\n")
            else:
                f.write("\n")

        f.write(f"\n#endif // {prefix}_H\n")

    print(f"Generated: {header_filename}")
    print(f"  Vertices : {vertex_count} ({float_count} floats)")
    print(f"  Indices  : {index_count} ({index_count // 3} triangles)")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python obj_to_header.py model.obj")
        sys.exit(1)
    obj_to_define_header(sys.argv[1])
