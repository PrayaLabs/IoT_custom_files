import tkinter as tk
from tkinter import scrolledtext
import paho.mqtt.client as mqtt

# MQTT client class with UI
class MQTTClientApp:
    def __init__(self, root):
        self.root = root
        self.root.title("MQTT Client")

        # Broker details
        self.broker_ip = tk.StringVar()
        self.broker_port = tk.IntVar(value=1883)
        self.pub_topic = tk.StringVar()
        self.sub_topic = tk.StringVar()
        self.message = tk.StringVar()

        # Setup UI
        self.setup_ui()

        # Initialize MQTT client
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def setup_ui(self):
        # IP Address
        tk.Label(self.root, text="Broker IP Address").grid(row=0, column=0)
        tk.Entry(self.root, textvariable=self.broker_ip).grid(row=0, column=1)

        # Port
        tk.Label(self.root, text="Port").grid(row=1, column=0)
        tk.Entry(self.root, textvariable=self.broker_port).grid(row=1, column=1)

        # Publish Topic
        tk.Label(self.root, text="Publish Topic").grid(row=2, column=0)
        tk.Entry(self.root, textvariable=self.pub_topic).grid(row=2, column=1)

        # Subscribe Topic
        tk.Label(self.root, text="Subscribe Topic").grid(row=3, column=0)
        tk.Entry(self.root, textvariable=self.sub_topic).grid(row=3, column=1)

        # Message Input
        tk.Label(self.root, text="Message").grid(row=4, column=0)
        tk.Entry(self.root, textvariable=self.message).grid(row=4, column=1)

        # Buttons
        button_frame = tk.Frame(self.root)
        button_frame.grid(row=5, column=0, columnspan=2)

        tk.Button(button_frame, text="Connect", command=self.connect_broker).pack(side=tk.LEFT, padx=5)
        tk.Button(button_frame, text="Publish", command=self.publish_message).pack(side=tk.LEFT, padx=5)
        tk.Button(button_frame, text="Clear", command=self.clear_messages).pack(side=tk.LEFT, padx=5)

        # Feedback Message Area
        tk.Label(self.root, text="Feedback").grid(row=6, column=0, columnspan=2)
        self.feedback_area = scrolledtext.ScrolledText(self.root, wrap=tk.WORD, width=50, height=5)
        self.feedback_area.grid(row=7, column=0, columnspan=2)

        # Received Messages Area
        tk.Label(self.root, text="Received Messages").grid(row=8, column=0, columnspan=2)
        self.message_area = scrolledtext.ScrolledText(self.root, wrap=tk.WORD, width=50, height=10)
        self.message_area.grid(row=9, column=0, columnspan=2)

    def connect_broker(self):
        try:
            self.client.connect(self.broker_ip.get(), self.broker_port.get(), 60)
            if self.sub_topic.get():
                self.client.subscribe(self.sub_topic.get())
            self.client.loop_start()
            self.display_feedback("Server Connected", "green")
        except Exception as e:
            self.display_feedback(f"Connection Error: {e}", "red")

    def publish_message(self):
        try:
            if self.pub_topic.get():
                self.client.publish(self.pub_topic.get(), self.message.get())
                self.display_feedback("Message published successfully", "green")
            else:
                self.display_feedback("Publish Error: Publish topic is empty", "red")
        except Exception as e:
            self.display_feedback(f"Publish Error: {e}", "red")

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.display_feedback("Connected with result code 0 (Success)", "green")
        else:
            self.display_feedback(f"Failed to connect, return code {rc}", "red")

    def on_message(self, client, userdata, msg):
        self.message_area.insert(tk.END, f"Received message: {msg.payload.decode()} on topic {msg.topic}\n")
        self.message_area.see(tk.END)  # Auto-scroll to the latest message

    def display_feedback(self, message, color):
        self.feedback_area.insert(tk.END, f"{message}\n")
        self.feedback_area.tag_add("feedback", "end-2l", "end-1l")
        self.feedback_area.tag_config("feedback", foreground=color)
        self.feedback_area.see(tk.END)  # Auto-scroll to the latest feedback

    def clear_messages(self):
        self.feedback_area.delete(1.0, tk.END)
        self.message_area.delete(1.0, tk.END)

# Main execution
if __name__ == "__main__":
    root = tk.Tk()
    app = MQTTClientApp(root)
    root.mainloop()
