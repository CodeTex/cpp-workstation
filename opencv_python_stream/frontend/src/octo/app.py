"""Main application module."""
import tkinter as tk
from tkinter import ttk

import cv2
from PIL import Image, ImageTk
from pygrabber.dshow_graph import FilterGraph


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Video Input Stream Selector")
        self.geometry("800x600")  # Adjusted for video display

        # Get video devices
        self.video_devices = self.get_video_devices()
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

        # Mirror checkbox
        self.apply_mirror = tk.BooleanVar(value=False)  # Default to not mirrored
        self.mirror_button = ttk.Checkbutton(
            self, text="Mirror Video", variable=self.apply_mirror, onvalue=True, offvalue=False
        )
        self.mirror_button.grid(column=2, row=0, padx=10, pady=10)

        # Start button
        self.start_button = ttk.Button(
            self, text="Start", command=self.start_processing
        )
        self.start_button.grid(column=3, row=0, padx=10, pady=10)


        # Video display
        self.video_label = tk.Label(self)  # This label will display the video
        self.video_label.grid(column=0, row=1, columnspan=3, padx=10, pady=10)

    def get_video_devices(self):
        graph = FilterGraph()
        return graph.get_input_devices()

    def start_processing(self):
        device_id = self.video_devices.index(self.selected_device.get())
        self.capture = cv2.VideoCapture(device_id)
        self.update_video()

    def update_video(self):
        _, frame = self.capture.read()
        if self.apply_gray.get():
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2RGB)  # Convert back to RGB for display
        if self.apply_mirror.get():
            frame = cv2.flip(frame, 1)  # Flip the frame horizontally
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # Convert BGR to RGB
        img = Image.fromarray(frame)
        imgtk = ImageTk.PhotoImage(image=img)
        self.video_label.imgtk = imgtk
        self.video_label.configure(image=imgtk)
        self.video_label.after(10, self.update_video)  # Refresh the video display every 10 ms

    def on_closing(self):
        if self.capture.isOpened():
            self.capture.release()
        self.destroy()
