from pathlib import Path
import shutil
import json
import os


def create_new_project(path: Path, name: str, engine_path: str):
    project_dir = path / name
    project_dir.mkdir()

    assets_dir = f'{project_dir}/assets'
    os.mkdir(assets_dir)
    os.mkdir(f"{assets_dir}/scenes")

    subs = ['materials', 'models', 'textures', 'shaders', 'fonts']
    for sub in subs:
        shutil.copytree(f'{engine_path}/assets/{sub}', f'{assets_dir}/{sub}')

    shutil.copy(
        f'{engine_path}/assets/scenes/test.starscene', f'{assets_dir}/scenes/test.starscene')

    config = {
        "window": {
            "width": 1920,
            "height": 1080
        },
        "layout": {
            "width": 0.20,
            "height": 0.3,
        },
        "assets-root": f'{project_dir}/assets',
        "initial-scene": "test.starscene"
    }

    with open(project_dir / 'config.json', 'w+') as f:
        f.write(json.dumps(config))


def run_project(path: Path, name: str, engine_path: str):
    exe_path = f'{engine_path}/build/Debug/editor/starlight-editor'
