def read_file_to_list(filename):
    with open(filename, 'r') as file:
        # Reading each line, stripping newlines, and splitting by space
        # Converting each part to integer, and slicing to keep only the first 10 items
        data = [list(map(int, line.strip().split()))[:10] for line in file.readlines()]
    return data

def main(input_filename, output_filename):
    # Read data from files
    input_data = read_file_to_list(input_filename)
    output_data = read_file_to_list(output_filename)

    # Set to hold the first 10 items of each row for quick lookup
    input_rows = set(tuple(row) for row in input_data)
    output_rows = set(tuple(row) for row in output_data)

    # Find unique rows in input not in output
    unique_input = input_rows - output_rows
    unique_output = output_rows - input_rows

    # Print unique rows in input that are not found in output
    if unique_input:
        print("Rows in input not in output:")
        for row in unique_input:
            print(' '.join(map(str, row)))
    else:
        print("No unique rows in input that are not in output.")

    # Print unique rows in output that are not found in input
    if unique_output:
        print("Rows in output not in input:")
        for row in unique_output:
            print(' '.join(map(str, row)))
    else:
        print("No unique rows in output that are not in input.")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: python script.py <inputfile> <outputfile>")
    else:
        main(sys.argv[1], sys.argv[2])
