from pathlib import Path
import shutil
import json
import os


def create_new_project(path: Path, name: str, engine_path: str):
    project_dir = path / name
    project_dir.mkdir()

    subs = ['materials', 'models', 'textures', 'shaders', 'fonts']

    for sub in subs:
        shutil.copytree(f'{engine_path}/assets/{sub}', f'{project_dir}/{sub}')

    os.mkdir(f"{project_dir}/scenes")
    shutil.copy(
        f'{engine_path}/assets/scenes/test.starscene.json', f'{project_dir}/scenes/test.starscene.json')

    config = {
        "window": {
            "width": 1920,
            "height": 1080
        },
        "layout": {
            "width": 0.20,
            "height": 0.25,
        },
        "project-root": str(project_dir),
        "initial-scene": "test.starscene.json"
    }

    with open(project_dir / 'config.json', 'w+') as f:
        f.write(json.dumps(config))


def run_project(path: Path, name: str, engine_path: str):
    exe_path = f'{engine_path}/build/Debug/editor/starlight-editor'
