import glob
import json
import logging
from pathlib import Path
import sys


def aggregate_reports(files: list[str]) -> list[dict]:
    """
    Aggregates each benchmark into a single list.

    Args:
        files (list[str]): The list benchmark reports to aggregate.

    Returns:
        list[dict]: A list of the aggregated benchmarks.
    """
    benchmarks = []

    for path in files:
        with path.open() as file:
            data = json.load(file)

        if len(data.get("benchmarks", [])) <= 0:
            continue

        result = {
            "iterations": None,
            "mean": None,
            "median": None,
            "name": None,
            "repetitions": None,
            "standard_deviation": None,
            "time_unit": None,
        }

        for benchmark in data.get("benchmarks", []):
            name = benchmark["name"].split("/")[0] # the name will be in the format "Naive/iterations:1000000/repeats:30"

            if benchmark["run_type"] == "iteration":
                if result["name"] is None:
                    result["name"] = benchmark["run_name"].split("/")[0]

                if result["repetitions"] is None:
                    result["repetitions"] = benchmark["repetitions"]

                if result["iterations"] is None:
                    result["iterations"] = benchmark["iterations"]

                if result["time_unit"] is None:
                    result["time_unit"] = benchmark["time_unit"]

            if benchmark["run_type"] == "aggregate":
                if benchmark["aggregate_name"] == "mean":
                    result["mean"] = benchmark["real_time"]

                if benchmark["aggregate_name"] == "median":
                    result["median"] = benchmark["real_time"]

                if benchmark["aggregate_name"] == "stddev":
                    result["standard_deviation"] = benchmark["real_time"]

        benchmarks.append(result)

    return benchmarks

def print_benchmark(benchmarks: list[dict], type: str) -> None:
    """
    Prints a formatted table of the benchmark results to the console.

    Args:
        benchmarks (list[dict]): The list of benchmarks to print.
        type (str): The type of benchmark, i.e. "Decryption" or "Encryption".
    """
    if len(benchmarks) <= 0:
        return None

    default_width = 15
    title = f"Benchmark ({type})"
    name_width = 30

    # print the header
    print("-" * (name_width + default_width + default_width + default_width + default_width))
    print(f"{title:<{name_width}}{'Mean':>{default_width}}{'Median':>{default_width}}{'Std Deviation':>{default_width}}{'Iterations':>{default_width}}")
    print("-" * (name_width + default_width + default_width + default_width + default_width))

    # print each benchmark row
    for benchmark in benchmarks:
        iterations = benchmark.get("iterations", 0)
        name = benchmark.get("name", "Unknown")
        time_unit = benchmark.get("time_unit", "ns")
        mean_time = f"{benchmark.get('mean', 0):.3f} {time_unit}"
        median_time = f"{benchmark.get('median', 0):.3f} {time_unit}"
        standard_deviation_time = f"{benchmark.get('standard_deviation', 0):.3f} {time_unit}"

        print(f"{name:<{name_width}}{mean_time:>{default_width}}{median_time:>{default_width}}{standard_deviation_time:>{default_width}}{iterations:>{default_width}}")

    print("-" * (name_width + default_width + default_width + default_width + default_width))

def main():
    reports_dir = Path(f"{Path(__file__).resolve().parent.parent}/.benchmarks")
    decryption_reports = sorted(reports_dir.glob("*_decrypt_benchmark.json"))
    encryption_reports = sorted(reports_dir.glob("*_encrypt_benchmark.json"))
    benchmarks = {
        "decryption": [],
        "encryption": [],
    }

    try:
        benchmarks["decryption"] =  aggregate_reports(decryption_reports)
        benchmarks["encryption"] = aggregate_reports(encryption_reports)
    except Exception as e:
        logging.error(e, exc_info=True)
        sys.exit(1)

    # save to a file
    with open(f"{reports_dir}/aggregated_benchmarks.json", "w") as f:
        json.dump(benchmarks, f, indent=4)

    print_benchmark(benchmarks["decryption"], "Decryption")
    print_benchmark(benchmarks["encryption"], "Encryption")

    sys.exit(0)

if __name__ == "__main__":
    main()
