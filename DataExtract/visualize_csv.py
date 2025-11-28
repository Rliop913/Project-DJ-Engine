#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def load_callbacks_csv(path: str) -> pd.DataFrame:
    """
    hybrid_callbacks.csv 를 DataFrame으로 로드.
    컬럼: index, phase, start_ts, end_ts, dur_us
    """
    df = pd.read_csv(path)
    # 혹시 타입이 엉켜있을 수 있으니 변환 한 번 해줌
    df["index"] = df["index"].astype(int)
    df["dur_us"] = df["dur_us"].astype(float)
    df["phase"] = df["phase"].astype(str)
    return df


def plot_timeline(df: pd.DataFrame,
                  max_us: float | None = None,
                  filename: str = "timeline_by_phase.png"):
    """
    x축: 콜백 index, y축: dur_us, 색: phase
    → 스파이크가 어느 phase 구간에서 자주 터지는지 한 번에 보기 좋음.
    """
    plt.figure(figsize=(12, 4))
    sns.scatterplot(
        data=df,
        x="index",
        y="dur_us",
        hue="phase",
        s=8,
        alpha=0.6,
    )

    plt.xlabel("Callback index")
    plt.ylabel("Duration (µs)")
    plt.title("Hybrid render callback duration over time (colored by phase)")

    if max_us is not None:
        plt.ylim(0, max_us)

    plt.grid(True, linestyle="--", alpha=0.3)
    plt.legend(title="Phase", bbox_to_anchor=(1.05, 1), loc="upper left")
    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    print(f"[+] 타임라인 그래프 저장: {filename}")


def plot_box_by_phase(df: pd.DataFrame,
                      min_count: int = 10,
                      filename: str = "boxplot_by_phase.png"):
    """
    phase별 dur_us 분포(boxplot).
    - 호출 횟수가 너무 적은 phase는 min_count 기준으로 필터링.
    - phase명을 호출 횟수/평균 시간 순으로 정렬해서 읽기 쉽게.
    """
    # phase별 count 계산
    cnt = df.groupby("phase")["dur_us"].count().rename("count")
    mean = df.groupby("phase")["dur_us"].mean().rename("mean_us")

    stats = pd.concat([cnt, mean], axis=1)

    # 일정 횟수 이상만 사용 (너무 희귀한 phase는 제외)
    stats = stats[stats["count"] >= min_count]

    if stats.empty:
        print(f"[!] min_count={min_count} 이상인 phase가 없어 boxplot을 건너뜁니다.")
        return

    # 평균 시간 기준으로 정렬 (느린 phase가 오른쪽으로 오게)
    stats = stats.sort_values("mean_us", ascending=True)
    top_phases = stats.index.tolist()

    df_sub = df[df["phase"].isin(top_phases)].copy()
    # phase 컬럼을 카테고리 + 정렬 순서 지정
    df_sub["phase"] = pd.Categorical(df_sub["phase"], categories=top_phases, ordered=True)

    plt.figure(figsize=(max(8, len(top_phases) * 0.8), 4))
    sns.boxplot(
        data=df_sub,
        x="phase",
        y="dur_us",
    )
    plt.xticks(rotation=30, ha="right")
    plt.xlabel("Phase (FLAG)")
    plt.ylabel("Duration (µs)")
    plt.title(f"Hybrid render callback duration by phase (n ≥ {min_count})")
    plt.grid(True, linestyle="--", axis="y", alpha=0.3)
    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    print(f"[+] phase별 boxplot 저장: {filename}")


def plot_bar_mean_by_phase(df: pd.DataFrame,
                           min_count: int = 10,
                           filename: str = "bar_mean_by_phase.png"):
    """
    phase별 평균 dur_us를 barplot으로 표현.
    boxplot보다 요약된 비교 그림이 필요할 때 사용.
    """
    cnt = df.groupby("phase")["dur_us"].count().rename("count")
    mean = df.groupby("phase")["dur_us"].mean().rename("mean_us")
    stats = pd.concat([cnt, mean], axis=1)
    stats = stats[stats["count"] >= min_count]

    if stats.empty:
        print(f"[!] min_count={min_count} 이상인 phase가 없어 barplot을 건너뜁니다.")
        return

    stats = stats.sort_values("mean_us", ascending=True)

    plt.figure(figsize=(max(8, len(stats) * 0.8), 4))
    sns.barplot(
        data=stats,
        x=stats.index,
        y="mean_us",
    )
    plt.xticks(rotation=30, ha="right")
    plt.xlabel("Phase (FLAG)")
    plt.ylabel("Mean duration (µs)")
    plt.title(f"Mean hybrid render duration by phase (n ≥ {min_count})")
    plt.grid(True, linestyle="--", axis="y", alpha=0.3)
    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    print(f"[+] phase별 평균 barplot 저장: {filename}")


def main():
    import os
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c",
        "--csv",
        type=str,
        default="hybrid_callbacks.csv",
        help="hybrid_callbacks.csv 경로 (기본: ./hybrid_callbacks.csv)",
    )
    parser.add_argument(
        "--max-us",
        type=float,
        default=None,
        help="타임라인 그래프 y축 최대값(µs). 지정 안하면 자동 스케일.",
    )
    parser.add_argument(
        "--min-count",
        type=int,
        default=10,
        help="boxplot / barplot에 포함할 phase의 최소 콜백 개수",
    )
    args = parser.parse_args()
    file_name = os.path.basename(args.csv)
    df = load_callbacks_csv(args.csv)
    print(df.head())

    plot_timeline(df, max_us=args.max_us, filename=f"{file_name}_timeline_by_phase.png")
    plot_box_by_phase(df, min_count=args.min_count, filename=f"{file_name}_boxplot_by_phase.png")
    plot_bar_mean_by_phase(df, min_count=args.min_count, filename=f"{file_name}_bar_mean_by_phase.png")


if __name__ == "__main__":
    main()
