import AdapterBoard 
import cv2 as cv 
import numpy as np  

Arducam_adapter_board = AdapterBoard.MultiAdapter()
if __name__ == "__main__":
    Arducam_adapter_board.init(320,240)
    Arducam_adapter_board.preview()

    