# typing — 터미널 타자 연습

macOS / Linux 터미널에서 돌아가는 영어 타자 연습 프로그램입니다 (C + ncurses).
한메타자처럼 자리 연습, 단어, 문장, 문단 연습과 떨어지는 단어를 맞히는 게임이 있습니다.

## 빌드

macOS (Homebrew):

```sh
brew install ncurses
make
```

Linux (Debian/Ubuntu):

```sh
sudo apt install build-essential clang libncurses-dev
make NCURSES_PREFIX=/usr BREW_PREFIX=/usr
```

`make clean` 으로 실행 파일을 지웁니다.

## 실행

```sh
make run        # 또는 ./main
```

- **저장소 폴더 안에서 실행하세요.** `text/` 폴더의 연습 문장을 상대 경로로 읽습니다.
- 터미널 크기는 **가로 120 × 세로 45 이상**을 권장합니다 (화면 레이아웃이 이 크기 기준).

## 조작

| 키 | 동작 |
|---|---|
| `↑` `↓` 또는 `k` `j` | 메뉴 이동 |
| `Enter` | 선택 / 한 줄 입력 끝 |
| `Backspace` | 지우기 |
| `Esc` | 연습 중간에 메뉴로 돌아가기 (게임 모드 제외) |

틀린 글자는 원문 위에 `v` 표시와 함께 삐 소리가 납니다. 한 줄을 끝내면 분당 타수가 나오고, `Enter` 또는 `Space` 로 다음으로 넘어갑니다.

## 메뉴

| 메뉴 | 내용 |
|---|---|
| 1. Basic | 자리 연습: 기본 자리 / 윗줄 / 아랫줄 / 특수문자 / 숫자 |
| 2. words | 영어 단어 7개씩 (`text/words.txt`) |
| 3. Sentences | 영어 문장 한 줄씩 (`text/sentence.txt`) |
| 4. Paragraph | 문단 5줄씩, 이어서 진행 (`text/paragraph.txt`, 끝나면 처음부터) |
| 5. Game | 위에서 떨어지는 단어를 입력하고 `Space`/`Enter` 로 맞히기. 100개가 다 떨어지면 끝 |
| 6. Option | 1~4번 연습의 반복 횟수 설정 |
| 7. Quit | 종료 |

게임 화면 아래 칸의 숫자는 `맞힌 개수  남은 목숨` 입니다. 맞힐수록 단어가 빨리 떨어집니다.

## 설정 파일

Option 에서 바꾼 반복 횟수는 `option.conf` 에 저장됩니다 (기초 / 단어 / 문장 / 문단 순서, 한 줄에 하나).
파일이 없으면 기본값 `10 / 10 / 15 / 5` 로 시작하고, 개인 설정이라 git 에는 올라가지 않습니다.

## 폴더 구성

```
main.c            프로그램 본체
include/          공용 헤더, 세마포어 래퍼 (macOS dispatch / POSIX)
text/             메뉴 문구와 연습 문장
text/extra/       현재 안 쓰는 예비 단어 목록
examples/m5.c     포인터/2차원 배열 연습 코드 (프로그램과 무관)
assets/Pop.aiff   효과음 (현재 안 씀)
docs/git-guide.md Git / GitHub 입문 노트
```

## 알려진 제한

- 게임 모드는 중간에 나갈 수 없습니다. 끝까지 하거나 `Ctrl+C` 로 종료하세요.
- 영어(ASCII)만 지원합니다. 한글 입력은 안 됩니다.
