"""Utility functions for the frontend app."""

import subprocess

from pygrabber.dshow_graph import FilterGraph


def get_video_devices():
    graph = FilterGraph()
    return graph.get_input_devices()


def start_video_processing(device_id, apply_gray):
    # Convert boolean to an integer for simplicity
    apply_gray_int = int(apply_gray)
    # Constructing command to execute C++ program
    command = ["./VideoProcessor", str(device_id), str(apply_gray_int)]
    subprocess.Popen(command)
