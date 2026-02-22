import sys
import os

def obj_to_define_header(obj_filename):
    vertices = []

    with open(obj_filename, 'r') as f:
        for line in f:
            if line.startswith('v '):
                parts = line.strip().split()
                vertices.extend([float(parts[1]), float(parts[2]), float(parts[3])])

    header_filename = os.path.splitext(obj_filename)[0] + ".h"

    base_name = os.path.splitext(os.path.basename(obj_filename))[0]
    macro_name = base_name.upper()

    float_count = len(vertices)

    with open(header_filename, 'w') as f:
        f.write(f"#ifndef {macro_name}_H\n")
        f.write(f"#define {macro_name}_H\n\n")
        f.write(f"#define {macro_name}_FLOATS {float_count}\n\n")
        f.write(f"#define {macro_name} \\\n")
        
        for i in range(0, float_count, 3):
            f.write(f"    {vertices[i]:.6f}f, {vertices[i+1]:.6f}f, {vertices[i+2]:.6f}f,")
            if i + 3 < float_count:
                f.write(" \\\n")
            else:
                f.write("\n")

        f.write(f"\n#endif // {macro_name}_H\n")

    print(f"Header file generated: {header_filename} with {float_count} floats.")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python obj_to_header.py model.obj")
        sys.exit(1)

    obj_file = sys.argv[1]
    obj_to_define_header(obj_file)
