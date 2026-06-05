import os
import shutil
import argparse
from pathlib import Path

source = Path(r'E:\002GitCode\occt_sdk\3rdparty-vc14-64-8.0\3rdparty-vc14-64').resolve()
target = Path(r'E:\002GitCode\occt_sdk\installed\win64\vc14\bind').resolve()
copied_count = 0
# 递归遍历并处理 DLL
for root, _, files in os.walk(source):
        for file in files:
            if file.lower().endswith('.dll'):
                src_path = Path(root) / file
                dest_path = target / file
                
                # 处理文件冲突
                if dest_path.exists():
                    continue
                else:
                    shutil.copy2(src_path, dest_path)
                
                copied_count += 1
                print(f"✅ 已拷贝: {file} ({src_path.relative_to(source)})")    