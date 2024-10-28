import argparse
import itertools
import logging
import subprocess
import yaml

from pathlib import Path
from typing import TypedDict, List

BASE_PATH = Path(__file__).absolute().parent
OUTPUT_FILE = "witness.output"  # File where C++ writes the sort output

LOG = logging.getLogger(__name__)
logging.basicConfig(
    level=logging.INFO, 
    format="%(asctime)s - %(levelname)s - %(message)s",
    datefmt='%Y-%m-%d %H:%M:%S'
)

class ExperimentParameters(TypedDict):
    row_num: List[int]
    value_range: List[int]
    predicates: List[str]


def load_experiment(exp_name: str):
    exp_path = BASE_PATH / "experiment" / f"{exp_name}.yaml"
    if not exp_path.exists():
        LOG.error(f"Experiment file {exp_path} not found.")
        return None

    with open(exp_path) as f:
        return yaml.safe_load(f)


def generate_combinations(params: ExperimentParameters):
    keys = params.keys()
    values = (params[key] if isinstance(params[key], list) else [params[key]] for key in keys)
    for combination in itertools.product(*values):
        yield dict(zip(keys, combination))


def run_experiment(params: ExperimentParameters):
    with open(OUTPUT_FILE, "w") as f:
        f.truncate(0)

    cmd_base = [
        "./build/Main.exe",
        f"--row_number={params['row_num']}",
        f"--value_range={params['value_range']}",
    ]

    if "predicates" in params and params["predicates"]:
        cmd_base.append(f"--predicates={params['predicates']}")

    LOG.info(f"Executing: {' '.join(cmd_base)}")

    try:
        subprocess.run(cmd_base, check=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError as e:
        LOG.error(f"Command failed: {e}")
        raise

def check_sort_output():
    """
    This function checks the parity and order output from the file.
    It assumes that the file contains exactly 4 lines:
    - The 1st line is the input order.
    - The 2nd line is the input parity.
    - The 3rd line is the output order.
    - The 4th line is the output parity.
    """
    try:
        with open(OUTPUT_FILE, "r") as file:
            lines = file.readlines()

            # Ensure there are exactly 4 lines
            if len(lines) != 4:
                LOG.error("Incomplete data in the file. Expected 4 lines but got fewer.")
                return

            # Extract relevant lines
            input_order = lines[0].strip().split()[-2]  # input True/False order
            output_order = lines[2].strip().split()[-2]  # output True/False order

            parity_before = lines[1].strip().split(":")[1].strip().split()[0]  # input Parity values
            parity_after = lines[3].strip().split(":")[1].strip().split()[0]   # output Parity values

            # Check if the input and output parity match
            if parity_before != parity_after:
                LOG.error("Parity mismatch between input and output.")

            # Check if output order is true
            if output_order != "true":
                LOG.error("Incorrect output order.")

            LOG.info(f"Input parity: {parity_before}")
            LOG.info(f"Output parity: {parity_after}")
            LOG.info(f"Order check: {output_order}")

    except FileNotFoundError:
        LOG.error(f"Output file '{OUTPUT_FILE}' not found.")

def main():
    parser = argparse.ArgumentParser(description="Run external-sort experiments with different parameters.")
    parser.add_argument("experiment", help="Experiment name to run.")
    args = parser.parse_args()

    exp_params = load_experiment(args.experiment)
    if exp_params is None:
        return

    for combination in generate_combinations(exp_params):
        try:
            run_experiment(combination)
            check_sort_output()  
        except subprocess.CalledProcessError as e:
            LOG.error(f"Experiment failed with error: {e}")


if __name__ == "__main__":
    main()
