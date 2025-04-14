import shutil
from pathlib import Path

import dearpygui.dearpygui as dpg

from .project import *


PROJECTS_PATH = '/home/nek0/starlight'
ENGINE_PATH = '/home/nek0/kapik/projects/starlight'
WIDTH = 620
HEIGHT = 440


class Launcher(object):
    def __init__(self):
        self.project_root = Path(PROJECTS_PATH)
        self.selected_project = None
        self.load_projects()

    def load_projects(self):
        self.projects = [
            p.name for p in self.project_root.iterdir() if p.is_dir()]

    def new_project_callback(self):
        print("New Project clicked")
        dpg.hide_item("MainWindow")
        dpg.show_item("NewProjectWindow")

    def back_to_main_callback(self):
        dpg.hide_item("NewProjectWindow")
        dpg.show_item("MainWindow")

    def create_new_project_callback(self):
        name = str(dpg.get_value('project_name_input'))
        print("Creating new project: `", name)
        create_new_project(self.project_root, name, ENGINE_PATH)
        self.populate_project_list()

    def load_project_callback(self):
        pass

    def on_project_selected(self, sender, app_data, user_data):
        if self.selected_project is not None:
            dpg.set_value(self.selected_project[1], False)

        dpg.set_value(sender, True)
        self.selected_project = (user_data, sender)
        print(f"Selected project: {user_data}")

    def populate_project_list(self):
        self.load_projects()
        dpg.delete_item("project_list", children_only=True)
        for project in self.projects:
            id = dpg.add_selectable(
                label=project,
                parent="project_list",
                callback=self.on_project_selected,
                user_data=project
            )
            if self.selected_project and self.selected_project[0] == project:
                self.selected_project = (project, id)

    def remove_project_callback(self):
        shutil.rmtree(self.project_root / self.selected_project[0])
        self.selected_project = None
        self.populate_project_list()

    def main_window(self):
        with dpg.window(
            tag="MainWindow",
            label="Starlight Launcher",
            no_move=True,
            no_resize=True,
            no_close=True,
            no_collapse=True,
            width=WIDTH,
            height=HEIGHT
        ):
            dpg.add_text("Projects")
            dpg.add_separator()
            dpg.add_spacer(width=2)

            with dpg.group(horizontal=True):
                with dpg.child_window(tag="project_list", width=400, height=385, border=True):
                    self.populate_project_list()

                with dpg.group():
                    dpg.add_button(label="New Project", width=190,
                                   callback=self.new_project_callback)
                    dpg.add_spacer(width=2)
                    dpg.add_button(label="Load Project", width=190,
                                   callback=self.load_project_callback)
                    dpg.add_spacer(width=2)
                    dpg.add_button(label="Remove Project", width=190,
                                   callback=self.remove_project_callback)

    def new_project_window(self):
        with dpg.window(
            tag="NewProjectWindow",
            label="Create New Project",
            show=False,
            width=WIDTH,
            height=HEIGHT,
            no_resize=True,
            no_move=True,
            no_close=True,
            no_collapse=True
        ):
            dpg.add_text("Enter Project Details")
            dpg.add_input_text(label="Project Name",
                               tag="project_name_input")
            dpg.add_separator()
            with dpg.group(horizontal=True):
                dpg.add_button(label="Create", width=100,
                               callback=self.create_new_project_callback)
                dpg.add_button(label="Back", width=100,
                               callback=self.back_to_main_callback)

    def run(self):
        dpg.create_context()

        self.main_window()
        self.new_project_window()

        dpg.create_viewport(title='Starlight Launcher', width=620, height=440)
        dpg.set_item_pos("MainWindow", [0, 0])
        dpg.set_primary_window("MainWindow", True)
        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.start_dearpygui()
        dpg.destroy_context()
