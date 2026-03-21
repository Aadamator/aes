import glob
import json
import logging
from pathlib import Path
import sys


def aggregate_reports(files: list[str]) -> dict:
    """
    Aggregates each benchmark into a single report that is saved to `
    .benchmarks/aggregated_benchmarks.json`.

    Args:
        files (list[str]): The list benchmark reports to aggregate.

    Returns:
        dict: The aggregated benchmarks.
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
            name = benchmark["name"]

            if benchmark["run_type"] == "iteration":
                if result["name"] is None:
                    result["name"] = benchmark["run_name"]

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

    return  {
        "benchmarks": benchmarks,
    }

def main() -> None:
    reports_dir = Path(f"{Path(__file__).resolve().parent.parent}/.benchmarks")
    reports = sorted(reports_dir.glob("*_benchmark.json"))

    try:
        benchmarks = aggregate_reports(reports)
    except Exception as e:
        logging.error(e, exc_info=True)
        sys.exit(1)

    with open(f"{reports_dir}/aggregated_benchmarks.json", "w") as f:
        json.dump(benchmarks, f, indent=4)

    sys.exit(0)

if __name__ == "__main__":
    main()
