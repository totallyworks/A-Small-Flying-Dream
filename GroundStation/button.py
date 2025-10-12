import pygame
import constants
from constants import WIN, W, H

def insideRect(pos, rectc, rects):
    if pos[0] >= rectc[0] - rects[0]/2 and pos[0] <= rectc[0] + rects[0]/2 and pos[1] >= rectc[1] - rects[1]/2 and pos[1] <= rectc[1] + rects[1]/2:
        return True
    else: return False

class Button:
    def __init__(self, text, size, center, hasParameterSaved, canDisplaySettings, value=None, color=constants.BLACK, bordercolor=constants.WHITE, activecolor=constants.GREY, textsize=30, textcolor=constants.WHITE, borderthickness=2, settingsDisplayed=False):
        self.text = text
        self.size = size
        self.center = center
        self.color = color
        self.bordercolor = bordercolor
        self.borderthickness = borderthickness
        self.textsize = textsize
        self.textcolor = textcolor
        self.activecolor = activecolor
        self.font = pygame.font.SysFont('Arial', self.textsize)
        self.value = value
        self.settingsDisplayed = settingsDisplayed
        self.hasParameterSaved = hasParameterSaved
        self.canDisplaySettings = canDisplaySettings

    def display(self):
        pygame.draw.rect(WIN, self.bordercolor, (self.center[0] - self.size[0]/2, self.center[1] - self.size[1]/2, self.size[0], self.size[1]))
        if insideRect(pygame.mouse.get_pos(), self.center, self.size):
            pygame.draw.rect(WIN, self.activecolor, (self.center[0] - self.size[0] / 2 + self.borderthickness, self.center[1] - self.size[1] / 2 + self.borderthickness, self.size[0] - 2 * self.borderthickness,self.size[1] - 2 * self.borderthickness))
        else:
            pygame.draw.rect(WIN, self.color, (self.center[0] - self.size[0] / 2 + self.borderthickness, self.center[1] - self.size[1] / 2 + self.borderthickness, self.size[0] - 2*self.borderthickness, self.size[1] - 2*self.borderthickness))

        text = self.font.render(self.text, 1, self.textcolor)
        textRect = text.get_rect()
        textRect.center = self.center
        WIN.blit(text, textRect)

    def isPressed(self):
        if insideRect(pygame.mouse.get_pos(), self.center, self.size):
            return True
        else: return False



