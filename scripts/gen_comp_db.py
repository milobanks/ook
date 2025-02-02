import sys
import json
import os

def parse_commands(input_lines):
    compile_commands = []

    for line in input_lines:
        line = line.strip()

        if line.startswith(os.environ['XCC']) or line.startswith(os.environ['XCPP']):
            # Ignore linker.
            if '-T ' in line:
                continue

            raw_args = line.split()
            args = [arg for arg in raw_args if not arg.startswith("-m")]

            output_index = args.index('-o') + 1 if '-o' in args else None
            source_file = args[-1]

            if output_index is not None:
                output_file = args[output_index]
                command_entry = {
                    "directory": os.getcwd(),
                    "command": ' '.join(args),
                    "file": source_file,
                    "output": output_file
                }
                compile_commands.append(command_entry)

    return compile_commands

def main():
    input_lines = sys.stdin.readlines()
    compile_commands = parse_commands(input_lines)
    print(json.dumps(compile_commands, indent=4))

if __name__ == "__main__":
    main()
