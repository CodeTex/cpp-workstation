"""Main application module."""
import tkinter as tk
from tkinter import ttk

from .utils import get_video_devices, start_video_processing


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Video Input Stream Selector")
        self.geometry("300x100")

        # Dropdown for video devices
        self.video_devices = get_video_devices()
        self.selected_device = tk.StringVar()
        self.device_dropdown = ttk.Combobox(
            self, textvariable=self.selected_device, values=self.video_devices
        )
        self.device_dropdown.grid(column=0, row=0, padx=10, pady=10)

        # Checkbox for grayscale filter
        self.apply_gray = tk.BooleanVar()
        self.gray_checkbox = ttk.Checkbutton(
            self, text="Apply Gray Filter", variable=self.apply_gray
        )
        self.gray_checkbox.grid(column=1, row=0, padx=10, pady=10)

        # Start button
        self.start_button = ttk.Button(
            self, text="Start", command=self.start_processing
        )
        self.start_button.grid(column=2, row=0, padx=10, pady=10)

    def start_processing(self):
        device_id = self.video_devices.index(self.selected_device.get())
        start_video_processing(device_id, self.apply_gray.get())
