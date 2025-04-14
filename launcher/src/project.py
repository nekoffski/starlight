from pathlib import Path
import shutil
import json


def create_new_project(path: Path, name: str, engine_path: str):
    project_dir = path / name
    project_dir.mkdir()

    subs = ['materials', 'models', 'textures', 'shaders', 'fonts']

    for sub in subs:
        shutil.copytree(f'{engine_path}/assets/{sub}', f'{project_dir}/{sub}')

    shutil.copy(
        f'{engine_path}/assets/scenes/test.starscene.json', project_dir)

    config = {
        "window": {
            "width": 1920,
            "height": 1080,
            "name": "Starlight"
        },
        "version": {
            "major": 0,
            "minor": 1,
            "build": 1
        },
        "paths": {
            "root": str(project_dir),
            "textures": f"{project_dir}/textures",
            "shaders": f"{project_dir}/shaders",
            "materials": f"{project_dir}/materials",
            "fonts": f"{project_dir}/fonts"
        }
    }

    with open(project_dir / 'config.json', 'w+') as f:
        f.write(json.dumps(config))


def run_project(path: Path, name: str, engine_path: str):
    exe_path = f'{engine_path}/build/Debug/editor/starlight-editor'
