#!/usr/bin/env python3
# Native ArdPortal fields are restored through a verified RAM-only boot transaction.
from pathlib import Path
import subprocess,sys
root=Path(__file__).resolve().parents[1]
subprocess.run([sys.executable,str(root.parent/'AC-ESP/tests/run_wire_tests.py')],check=True)
subprocess.run([sys.executable,str(root.parent/'AC-ESP/tests/run_replication_tests.py')],check=True)
