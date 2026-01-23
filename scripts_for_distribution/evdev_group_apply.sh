#!/usr/bin/env bash
set -euo pipefail

# evdev_group_apply.sh
# Usage: ./evdev_group_apply.sh   (fixed)

# ---------- helpers ----------
die() { echo "[ERR] $*" >&2; exit 1; }

pick_first_evdev() {
  # select first event*
  if compgen -G "/dev/input/event*" >/dev/null; then
    ls -1 /dev/input/event* 2>/dev/null | head -n1
    return 0
  fi
  return 1
}

resolve_gid_to_groupname() {
  local gid="$1"
  local name
  name="$(getent group | awk -F: -v g="$gid" '$3==g {print $1; exit}')"
  if [ -n "${name:-}" ]; then
    echo "$name"
    return 0
  fi
  return 1
}

confirm_terms_yn() {
  cat <<'EOF'
============================== TERMS OF USE / NOTICE ==============================

This script modifies system-level user/group configuration in order to grant access
to Linux input event devices (evdev, e.g. /dev/input/event*).

By proceeding, you acknowledge and agree that:

1) SECURITY / PRIVACY RISK
   Granting access to evdev may allow software to read raw input events. Depending on
   the device, this can include sensitive input (e.g. keystrokes). You must understand
   the implications and only proceed in a trusted environment.

2) USER RESPONSIBILITY
   You are solely responsible for any consequences resulting from running this script,
   including (but not limited to) security issues, privacy breaches, system misbehavior,
   loss of data, or policy violations.

3) NO WARRANTY
   This script is provided "AS IS", without warranty of any kind, express or implied.
   The author(s) and distributor(s) assume no liability for damages or claims arising
   from its use.

4) REVERSIBILITY
   Group membership changes typically require logout/login (or restarting affected
   applications/sessions) to take full effect. Reverting also requires modifying group
   membership accordingly.

==================================================================================
EOF

  local ans
  while true; do
    read -r -p "Proceed? [y/N]: " ans
    ans="${ans:-N}"
    case "$ans" in
      [yY]|[yY][eE][sS]) return 0 ;;
      [nN]|[nN][oO]) echo "[INFO] Cancelled by user."; return 1 ;;
      *) echo "Please answer y or n." ;;
    esac
  done
}


TARGET_USER="${SUDO_USER:-${USER:-$(id -un)}}"

DEVNODE="$(pick_first_evdev)" || die "No /dev/input/event* found (container/headless/driver not loaded?)."

# devnode group info
GID="$(stat -c '%g' "$DEVNODE")"
GROUP_NAME="$(stat -c '%G' "$DEVNODE")"

# If group name can't be resolved and shows numeric gid, try resolving via getent
if [[ "$GROUP_NAME" =~ ^[0-9]+$ ]]; then
  if RESOLVED="$(resolve_gid_to_groupname "$GID")"; then
    GROUP_NAME="$RESOLVED"
  else
    GROUP_NAME="evdev_gid_${GID}"
    echo "[WARN] No existing group entry for gid=$GID. A new group '$GROUP_NAME' will be created with gid=$GID."
    if ! confirm_terms_yn; then exit 0; fi
    sudo groupadd -g "$GID" "$GROUP_NAME"
    echo "[OK] Group created."
    echo "[ACTION] Adding user '$TARGET_USER' to group '$GROUP_NAME'..."
    sudo usermod -aG "$GROUP_NAME" "$TARGET_USER"
    echo "[DONE] Completed."
    echo "Note: You may need to log out/in (or restart apps) for changes to take effect."
    exit 0
  fi
fi

# Already in group?
if id -nG "$TARGET_USER" | tr ' ' '\n' | grep -qx "$GROUP_NAME"; then
  echo "[OK] User '$TARGET_USER' is already in group '$GROUP_NAME'. No changes made."
  exit 0
fi

echo "[INFO] This will add user '$TARGET_USER' to group '$GROUP_NAME' (derived from an evdev device)."
if ! confirm_terms_yn; then exit 0; fi

echo "[ACTION] Adding user '$TARGET_USER' to group '$GROUP_NAME'..."
sudo usermod -aG "$GROUP_NAME" "$TARGET_USER"

echo "[DONE] Completed."
echo "Note: You may need to log out/in (or restart apps) for changes to take effect."
