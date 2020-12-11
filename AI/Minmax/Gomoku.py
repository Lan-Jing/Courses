import sys
import pygame

from checkerboard import *
import ai_greedy as ag
import ai_minmax as am

def _get_init_player():
    flag = input("输入 0（人）/ 1（电脑）指定先手（执黑）: ")
    if flag == "0":
        return BLACK_CHESSMAN, 0
    elif flag == "1":
        return WHITE_CHESSMAN, 1
    else:
        return None, None

def _get_next(cur_runner):
    if cur_runner == BLACK_CHESSMAN:
        return WHITE_CHESSMAN
    else:
        return BLACK_CHESSMAN

maxDepth = 4
initList = []

def main():
    # initialize the game
    winner = None
    cur_runner, skip_flag = _get_init_player()
    if skip_flag == None:
        return
    # computer = ag.AI(Line_Points, WHITE_CHESSMAN if cur_runner == BLACK_CHESSMAN else BLACK_CHESSMAN)
    computer = am.AI(Line_Points, WHITE_CHESSMAN if cur_runner == BLACK_CHESSMAN else BLACK_CHESSMAN, maxDepth)
    checkerboard = Checkerboard(Line_Points)

    initList.append(((Line_Points-1)/2, (Line_Points-1)/2))
    initList.append(((Line_Points-1)/2, (Line_Points-1)/2 + 1))
    initList.append(((Line_Points-1)/2, (Line_Points-1)/2 - 1))
    initList.append(((Line_Points-1)/2 + 1, (Line_Points-1)/2))
    print("Initialized Positions:", initList)

    tmp_runner = BLACK_CHESSMAN
    for i in range(len(initList)):
        click_point = Point((int)(initList[i][1]), (int)(initList[i][0]))
        if i >= 2:
            tmp_runner = WHITE_CHESSMAN
        checkerboard.drop(tmp_runner, click_point)
        computer._checkerboard[(int)(initList[i][0])][(int)(initList[i][1])] = 2 if i >= 2 else 1

    human_win_count = 0
    machine_win_count = 0

    # plot the checkerboard
    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption('五子棋')

    font1 = pygame.font.SysFont('SimHei', 32)
    font2 = pygame.font.SysFont('SimHei', 72)
    fwidth, fheight = font2.size('黑方获胜')

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                sys.exit()
            elif event.type == KEYDOWN and event.key == K_RETURN and winner is not None:
                # start a new game
                winner = None
                cur_runner, skip_flag = _get_init_player()
                if skip_flag == None:
                    return 
                # computer = ag.AI(Line_Points, WHITE_CHESSMAN if cur_runner == BLACK_CHESSMAN else BLACK_CHESSMAN)
                computer = am.AI(Line_Points, WHITE_CHESSMAN if cur_runner == BLACK_CHESSMAN else BLACK_CHESSMAN, maxDepth)
                checkerboard = Checkerboard(Line_Points)
            elif event.type == MOUSEBUTTONDOWN and winner is None:
                # the human player moves. 
                if skip_flag == 1:
                    # skip the first move if machine goes first
                    skip_flag = 0
                    click_point = Point(-1, -1)
                else:
                    # Need to check the clicked position
                    pressed_array = pygame.mouse.get_pressed()
                    if pressed_array[0] == 0:
                        break

                    mouse_pos = pygame.mouse.get_pos()
                    click_point = get_clickpoint(mouse_pos)
                    if click_point is None:
                        print("落子位置超出棋盘区域")
                        break
                    
                    if checkerboard.can_drop(click_point) == 0:
                        break

                    winner = checkerboard.drop(cur_runner, click_point)
                    computer.get_opponent_drop(click_point)
                    print("人类落子位置：{}， 评分：{}".format(click_point, computer._get_point_score(click_point)))
                    if winner is not None:
                        human_win_count += 1
                        break
                cur_runner = _get_next(cur_runner)

                # the AI moves. It updates the status then give a strategy
                score, AI_point = computer.AI_drop()
                print("电脑落子位置：{}， 评分：{}".format(AI_point, score))
                winner = checkerboard.drop(cur_runner, AI_point)
                if winner is not None:
                    machine_win_count += 1
                    break
                cur_runner = _get_next(cur_runner)

        # draw the checkerboard and the pieces
        draw_checkerboard(screen)
        draw_right_info(screen, font1, cur_runner, human_win_count, machine_win_count)
        for i, row in enumerate(checkerboard.checkerboard):
            for j, cell in enumerate(row):
                if cell == BLACK_CHESSMAN.Value:
                    draw_chessman(screen, Point(j, i), BLACK_CHESSMAN.Color)
                elif cell == WHITE_CHESSMAN.Value:
                    draw_chessman(screen, Point(j, i), WHITE_CHESSMAN.Color)

        if winner:
            print_text(screen, font2, (SCREEN_WIDTH - fwidth)//2, (SCREEN_HEIGHT - fheight)//2, winner.Name + '获胜', RED_COLOR)

        pygame.display.flip()

if __name__ == '__main__':
    main()