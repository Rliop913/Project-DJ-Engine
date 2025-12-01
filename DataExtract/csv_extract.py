import argparse
import os
import csv
from collections import defaultdict

def parse_log(path: str):
    """benchRes.txt를 읽어서 (label, timestamp) 리스트로 반환"""
    records = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            # "meta,time" 형식 가정
            try:
                meta, t_str = line.rsplit(",", 1)
            except ValueError:
                # 콤마가 없으면 스킵
                continue
            try:
                ts = int(t_str)
            except ValueError:
                continue
            records.append((meta, ts))
    return records

def extract_hybrid_callbacks(records, flagName):
    """
    FLAG 구간을 기준으로 hybridrender callback calc start/end 쌍의 duration을 추출.
    return: list of dicts
      - phase      : 현재 FLAG 이름 (없으면 "NO_FLAG")
      - index      : 전체 콜백 순서
      - start_ts   : 시작 타임스탬프
      - end_ts     : 끝 타임스탬프
      - dur_us     : (end - start) [us]
    """
    callbacks = []
    current_phase = "NO_FLAG"
    current_start = None
    callback_idx = 0

    for meta, ts in records:
        # FLAG 라인: phase 전환
        if meta.startswith("FLAG "):
            # 예: "FLAG LoadMusic()" -> "LoadMusic()"
            phase_name = meta[len("FLAG "):].strip()
            if not phase_name:
                phase_name = meta  # 혹시 비어있으면 전체를 phase로
            current_phase = phase_name
            continue

        if meta == f"{flagName} callback calc start":
            current_start = ts
        elif meta == f"{flagName} callback calc end":
            if current_start is None:
                # 짝이 안 맞으면 무시
                continue
            dur = ts - current_start
            callbacks.append(
                {
                    "phase": current_phase,
                    "index": callback_idx,
                    "start_ts": current_start,
                    "end_ts": ts,
                    "dur_us": dur,
                }
            )
            callback_idx += 1
            current_start = None

    return callbacks

def save_callbacks_csv(callbacks, out_path="hybrid_callbacks.csv"):
    """각 콜백별 상세 데이터를 CSV로 저장"""
    if not callbacks:
        print("No callbacks found.")
        return

    fieldnames = ["index", "phase", "start_ts", "end_ts", "dur_us"]
    with open(out_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for c in callbacks:
            writer.writerow(c)
    print(f"[+] 콜백 상세 데이터를 {out_path} 로 저장했습니다.")

def save_phase_stats_csv(callbacks, out_path="hybrid_phase_stats.csv"):
    """FLAG phase별로 통계치(mean/min/max/count)를 CSV로 저장"""
    if not callbacks:
        print("No callbacks for stats.")
        return

    by_phase = defaultdict(list)
    for c in callbacks:
        by_phase[c["phase"]].append(c["dur_us"])

    fieldnames = ["phase", "count", "mean_us", "min_us", "max_us"]
    with open(out_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for phase, arr in by_phase.items():
            cnt = len(arr)
            mean_us = sum(arr) / cnt
            min_us = min(arr)
            max_us = max(arr)
            writer.writerow(
                {
                    "phase": phase,
                    "count": cnt,
                    "mean_us": f"{mean_us:.2f}",
                    "min_us": min_us,
                    "max_us": max_us,
                }
            )
    print(f"[+] phase 통계 데이터를 {out_path} 로 저장했습니다.")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-p",
        "--path",
        type=str,
        default="benchRes.txt",
        help="입력 로그 파일 경로 (기본: benchRes.txt)",
    )
    parser.add_argument(
        "--cb_name",
        type=str,
        default="hybridrender",
        help="callback name"
    )
    args = parser.parse_args()

    log_path = args.path
    if not os.path.isfile(log_path):
        print(f"입력 파일을 찾을 수 없습니다: {log_path}")
        return

    records = parse_log(log_path)
    print(f"[+] 총 {len(records)} 개의 레코드를 읽었습니다.")

    callbacks = extract_hybrid_callbacks(records, args.cb_name)
    print(f"[+] hybridrender callback calc 구간 {len(callbacks)} 개를 추출했습니다.")

    file_name = os.path.basename(args.path)
    save_callbacks_csv(callbacks, f"{file_name}_callbacks.csv")
    save_phase_stats_csv(callbacks, f"{file_name}_phase_stats.csv")

if __name__ == "__main__":
    main()
