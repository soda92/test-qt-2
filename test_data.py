import os
from pathlib import Path
CUR = Path(__file__).resolve().parent
conf = Path.joinpath(CUR, "config", "config.txt")
if not conf.is_file():
    conf = Path.joinpath(CUR, "config", "config.example.txt")

lines = []
with open(conf, encoding="utf-8", mode='r') as f:
    lines = f.readlines()
    lines = [line.replace("\n", "") for line in lines]

conf_dest = lines[0].split("=")[1]
conf_src = lines[1].split("=")[1]

ORI_DATA_DIR = Path.joinpath(CUR, "data", conf_src)
RST_DATA_DIR = Path.joinpath(CUR, "build", "dist", conf_dest)


def test_data():
    l1 = set(os.listdir(ORI_DATA_DIR))
    l2 = set(os.listdir(RST_DATA_DIR))
    assert(l1 == l2)

    for folder in l1:
        files1 = set(os.listdir(Path.joinpath(ORI_DATA_DIR, folder)))
        files2 = set(os.listdir(Path.joinpath(RST_DATA_DIR, folder)))
        assert(files1 == files2)

        for file in files1:
            file1_path = Path.joinpath(ORI_DATA_DIR, folder, file)
            file2_path = Path.joinpath(RST_DATA_DIR, folder, file)

            with open(file1_path, encoding="utf8", mode="r") as f1:
                with open(file2_path, encoding="utf8", mode="r") as f2:
                    content1 = f1.readlines()
                    content2 = f2.readlines()

                    assert(content1 == content2)

