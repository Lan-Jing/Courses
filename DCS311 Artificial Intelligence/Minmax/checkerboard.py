import pygame.gfxdraw
from pygame.locals import *
from collections import namedtuple

Chessman = namedtuple('Chessman', 'Name Value Color')
Point = namedtuple('Point', 'X Y')

BLACK_CHESSMAN = Chessman('黑子', 1, (45, 45, 45))
WHITE_CHESSMAN = Chessman('白子', 2, (219, 219, 219))

offset = [(1, 0), (0, 1), (1, 1), (1, -1)]

SIZE = 30         # 棋盘每个点时间的间隔
Line_Points = 13  # 棋盘每行/每列点数
Outer_Width = 20  # 棋盘外宽度
Border_Width = 4  # 边框宽度
Inside_Width = 4  # 边框跟实际的棋盘之间的间隔
                  # 边框线的长度
Border_Length = SIZE * (Line_Points - 1) + Inside_Width * 2 + Border_Width  
                  # 网格线起点（左上角）坐标
Start_X = Start_Y = Outer_Width + int(Border_Width / 2) + Inside_Width      
SCREEN_HEIGHT = SIZE * (Line_Points - 1) + Outer_Width * 2 + Border_Width + Inside_Width * 2  
SCREEN_WIDTH = SCREEN_HEIGHT + 200  

Stone_Radius = SIZE // 2 - 3 
Stone_Radius2 = SIZE // 2 + 3
Checkerboard_Color = (0xE3, 0x92, 0x65) 
BLACK_COLOR = (0, 0, 0)
WHITE_COLOR = (255, 255, 255)
RED_COLOR = (200, 30, 30)
BLUE_COLOR = (30, 30, 200)

RIGHT_INFO_POS_X = SCREEN_HEIGHT + Stone_Radius2 * 2 + 10

def print_text(screen, font, x, y, text, fcolor=(255, 255, 255)):
    imgText = font.render(text, True, fcolor)
    screen.blit(imgText, (x, y))

def draw_checkerboard(screen):
    screen.fill(Checkerboard_Color)
    pygame.draw.rect(screen, BLACK_COLOR, (Outer_Width, Outer_Width, Border_Length, Border_Length), Border_Width)
    for i in range(Line_Points):
        pygame.draw.line(screen, BLACK_COLOR,
                         (Start_Y, Start_Y + SIZE * i),
                         (Start_Y + SIZE * (Line_Points - 1), Start_Y + SIZE * i),
                         1)
    for j in range(Line_Points):
        pygame.draw.line(screen, BLACK_COLOR,
                         (Start_X + SIZE * j, Start_X),
                         (Start_X + SIZE * j, Start_X + SIZE * (Line_Points - 1)),
                         1)

def draw_chessman(screen, point, stone_color):
    pygame.gfxdraw.aacircle(screen, Start_X + SIZE * point.X, Start_Y + SIZE * point.Y, Stone_Radius, stone_color)
    pygame.gfxdraw.filled_circle(screen, Start_X + SIZE * point.X, Start_Y + SIZE * point.Y, Stone_Radius, stone_color)

def draw_right_info(screen, font, cur_runner, human_win_count, machine_win_count, ):
    print_text(screen, font, SCREEN_HEIGHT, SCREEN_HEIGHT - Stone_Radius2 * 8, '统计：', BLUE_COLOR)
    print_text(screen, font, SCREEN_HEIGHT, SCREEN_HEIGHT - int(Stone_Radius2 * 5.5) + 3, f'玩家 {human_win_count} 胜', BLUE_COLOR)
    print_text(screen, font, SCREEN_HEIGHT, SCREEN_HEIGHT - Stone_Radius2 * 3 + 3, f'电脑 {machine_win_count} 胜', BLUE_COLOR)

def get_clickpoint(click_pos):
    pos_x = click_pos[0] - Start_X
    pos_y = click_pos[1] - Start_Y
    if pos_x < -Inside_Width or pos_y < -Inside_Width:
        return None
    x = pos_x // SIZE
    y = pos_y // SIZE
    if pos_x % SIZE > Stone_Radius:
        x += 1
    if pos_y % SIZE > Stone_Radius:
        y += 1
    if x >= Line_Points or y >= Line_Points:
        return None

    return Point(x, y)

class Checkerboard:
    def __init__(self, line_points):
        self._line_points = line_points
        self._checkerboard = [[0] * line_points for _ in range(line_points)]

    def _get_checkerboard(self):
        return self._checkerboard

    checkerboard = property(_get_checkerboard)

    def can_drop(self, point):
        return self._checkerboard[point.Y][point.X] == 0

    def drop(self, chessman, point):
        self._checkerboard[point.Y][point.X] = chessman.Value

        if self._win(point):
            print(f'{chessman.Name}获胜，按下回车以重新开始')
            return chessman

    def _win(self, point):
        cur_value = self._checkerboard[point.Y][point.X]
        for os in offset:
            if self._get_count_on_direction(point, cur_value, os[0], os[1]):
                return True

    def _get_count_on_direction(self, point, value, x_offset, y_offset):
        count = 1
        for step in range(1, 5):
            x = point.X + step * x_offset
            y = point.Y + step * y_offset
            if 0 <= x < self._line_points and 0 <= y < self._line_points and self._checkerboard[y][x] == value:
                count += 1
            else:
                break
        for step in range(1, 5):
            x = point.X - step * x_offset
            y = point.Y - step * y_offset
            if 0 <= x < self._line_points and 0 <= y < self._line_points and self._checkerboard[y][x] == value:
                count += 1
            else:
                break

        return count >= 5