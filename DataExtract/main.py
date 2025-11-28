import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns


def load_durations(path: str, start_key="start", end_key="end"):
    """
    benchRes.txt에서 start/end 쌍을 찾아 duration(µs) 리스트를 반환.
    형식: "메타,타임스탬프" (타임스탬프는 마이크로초 정수)
    """
    rows = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                meta, t = line.rsplit(",", 1)
                rows.append((meta, int(t)))
            except ValueError:
                continue

    durations = []
    current_start = None
    for meta, t in rows:
        if start_key in meta:
            current_start = t
        elif end_key in meta and current_start is not None:
            durations.append(t - current_start)
            current_start = None

    return np.array(durations, dtype=float)

def plot_spike_timeline_seaborn(durations, filename="spike_timeline_seaborn.png",
                                ylim_us=None, use_log_y=False):
    idx = np.arange(len(durations))
    df = pd.DataFrame({
        "index": idx,
        "duration_us": durations,
    })

    plt.figure(figsize=(10, 4))
    sns.scatterplot(data=df, x="index", y="duration_us", s=5, alpha=0.5)
    plt.xlabel("Callback index")
    plt.ylabel("Duration (µs)")
    plt.title("Callback duration over time (scatter)")

    if ylim_us is not None:
        plt.ylim(0, ylim_us)
    if use_log_y:
        plt.yscale("log")

    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    print(f"[+] 스파이크 타임라인(seaborn) 저장: {filename}")

def main():
    import argparse
    import os

    parser = argparse.ArgumentParser()
    parser.add_argument("path", help="benchRes.txt path")
    parser.add_argument("--start-key", default="start")
    parser.add_argument("--end-key", default="end")
    parser.add_argument("--max-us", type=int, default=50,
                        help="그래프 X축 상한 (µs)")
    args = parser.parse_args()

    durations = load_durations(args.path, args.start_key, args.end_key)
    data_name = os.path.basename(args.path)
    
    
    
    if durations.size == 0:
        print("start/end 쌍을 찾지 못했습니다.")
        return

    print(f"유효한 구간 개수: {len(durations)}")

    # pandas 시리즈로 변환하면 seaborn에 바로 먹이기 편함
    s = pd.Series(durations, name="duration_us")

    # 요약 통계
    print("\n[요약 통계] (단위: µs)")
    print(f"min    : {s.min():.3f}")
    print(f"max    : {s.max():.3f}")
    print(f"mean   : {s.mean():.3f}")
    print(f"median : {s.median():.3f}")
    print(f"p90    : {s.quantile(0.90):.3f}")
    print(f"p95    : {s.quantile(0.95):.3f}")
    print(f"p99    : {s.quantile(0.99):.3f}")

    # -----------------------
    # 1) 히스토그램
    # -----------------------
    plt.figure()
    sns.histplot(s[s <= args.max_us], bins=args.max_us, kde=False)
    plt.xlabel("Duration (µs)")
    plt.ylabel("Count")
    plt.title(f"Duration Histogram (0–{args.max_us} µs)")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"hist_duration_{data_name}.png", dpi=150)
    print(f"[+] 히스토그램 저장: hist_duration_{data_name}.png")

    # -----------------------
    # 2) CDF (ECDF plot)
    # -----------------------
    plt.figure()
    sns.ecdfplot(s)
    plt.xlabel("Duration (µs)")
    plt.ylabel("Cumulative probability")
    plt.title("Duration CDF")
    plt.xlim(0, args.max_us)  # 꼬리는 텍스트로 설명
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"cdf_duration_{data_name}.png", dpi=150)
    print(f"[+] CDF 저장: cdf_duration_{data_name}.png")

    plot_spike_timeline_seaborn(durations, filename=f"spike_timeline_{data_name}.png")

if __name__ == "__main__":
    main()