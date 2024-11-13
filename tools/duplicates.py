def read_file_to_list(filename):
    with open(filename, 'r') as file:
        # Reading each line, stripping newlines, and splitting by space
        # Converting each part to integer, and slicing to keep only the first 10 items
        data = [list(map(int, line.strip().split()))[:10] for line in file.readlines()]
    return data

def find_duplicates(data):
    seen = set()
    duplicates = set()
    for row in data:
        row_tuple = tuple(row)
        if row_tuple in seen:
            duplicates.add(row_tuple)
        seen.add(row_tuple)
    return duplicates

def main(input_filename, output_filename):
    # Read data from files
    input_data = read_file_to_list(input_filename)
    output_data = read_file_to_list(output_filename)

    # Find duplicates in both files
    input_duplicates = find_duplicates(input_data)
    output_duplicates = find_duplicates(output_data)

    # Print duplicates in input
    if input_duplicates:
        print("Duplicate rows in input:")
        for row in input_duplicates:
            print(' '.join(map(str, row)))
    else:
        print("No duplicates in input.")

    # Print duplicates in output
    if output_duplicates:
        print("Duplicate rows in output:")
        for row in output_duplicates:
            print(' '.join(map(str, row)))
    else:
        print("No duplicates in output.")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: python script.py <inputfile> <outputfile>")
    else:
        main(sys.argv[1], sys.argv[2])
