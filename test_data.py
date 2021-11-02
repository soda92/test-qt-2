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

DATA_DIR = Path.joinpath(CUR, conf_dest)
ORI_DATA_DIR = Path.joinpath(CUR, "data", conf_src)

