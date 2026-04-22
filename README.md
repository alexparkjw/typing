# 1. GitHub 이란

GitHub란 웹기반의 분산 버전관리 호스팅 서비스이다. 

쉽게, 내가 작업한 것을 웹에 올려놓고, 그때 그때 수정한 것을 업데이트 하면, 그 업데이트한 버전별로 내용이 모두 보관되어, 무엇이 바꿨는지, 쉽게 관리할 수 있게 해준다.
뿐만아니라, 여러명이 한 프로젝트를 접급할 수 있어 개발에 유용하다. 한 작업을 동시에 여럿이 수정할 수 있고, 서로 다른 수정본을 비교하며, 우수한것을 선택할 수 있는 이점이 있고, 문제가 생겼을때, 과거로 추적하여 문제의 근원도 추적할 수 있다.

프로젝트 개발시에 고용주의 요구사항이나, 팀의 조화를 이루는데 상당히 유용하며, 그 유지 보수 관리에도 상당히 최적한 환경이 된다.
서버에 문제가 생겼을때, 자료 백업을 서버에만 의존하는게 아니고 분산관리 되기 때문에 복구에도 유용하며, 인터넷이 연결이 없을때도 작업을 할 수 있는 이점도 있다.
언제, 어디서, 누구든, 어떤기기에서도, 인터넷 연결없이도 작업 할 수 있는 환경을 제공하는 호스팅 서비스라고 보면된다. 

교육/학습 에서도 그 활용도가 아주 높다.어떤 문제해결에 있어서 나의 코드의 잘못 된 부분에 대한 조언도 받을 수 있으며, 남이 한 세련된 코드도 감상 할 수 있다.
또 자신의 프로젝트를 키워가기도 좋고, 그 배포에도 용이하다 

이제 거의 대부분의 프로젝트는 GitHub 을 이용한다고 보면된다. 공개 작업공간은 무료이고, 비공개 작업공간은 유료이다.



# 2. GitHub 설치

a. linux 설치 :
 - 페도라 계열은 `sudo yum install git-all` 실행
 - 데비안 계열은 `sudo apt-get install git-all` 실행

b. osx 설치 : 
 - xcode 를 앱스토어에서 설치한 후 , 
 - 터미널에서 `xcode-select --install` 실행

c. windows 설치 : 아래링크로 가면 자동 다운로드 후 설치
 - http://git-scm.com/download/win 



# 3. GitHub 간단한 사용예

a. GitHub 에 가입한다: 아이디,패스워드,이메일을 등록하구 , 이메일가서 확인을 하면 가입이 된다.

b. git repository 생성 : 웹페이지에서 new repository 클릭한후 repository name 을 지어주고create repository 버튼을 눌러서 만든다. (여기선 아이디는 alexparkjw 이고 리파지토리 이름은 typing 이라고 임으로 만들었다)

터미널에서 
```
echo "# typing" >> README.md
git init
git add README.md
git commit -m "first commit"
git remote add origin https://github.com/alexparkjw/typing.git
git push -u origin master
```

단, 본인의 아이디와, 본인이 정한 리파지토리 이름으로 바꿔줘야한다.

https://github.com/alexparkjw/typing.git 주소를 https://github.com/본인의아이디/리파지토리이름.git  이렇게 말이다.

위의 명령어는 본인의 컴퓨터 안에 작업할 디렉토리 안에서 해주면 .git/ 이라는 서브 디렉토리가 생성된다.



c. 디렉토리 안에서 작업을 한다: 예를들어 아래처럼 README.md 를 만들고 싶다면
```
1. GitHub 이란
...
2. GitHub 설치
...
3. GitHub 간단한 사용예
...

```



d. 생성된 파일(여기선 README.md 파일)을 스테이지에 올리는 명령어
```
git add README.md
```
 - 작업을 마치고,  명령어를 입럭한다.
```
git add .
```
 - 명령어로 현제 디렉토리의 모든 파일을 지목할 수도 있다.
```
git commit -m "create README.md"
```
- "코맨트"는 생략하면 안된다. 이 내용을 기반으로 해시 테이블에 넣어서 작업내용을 분류하기 때문이다.




e. 리파지토리에 올리는 명령어
```
git push -u origin master
```
- 마스터가 원본을 수정하고, 올린다는 뜻이다. 이 마스터 등급은 기본값이고, 여러 등급으로 나눌 수 있다...


f. 다른 작업공간에서 리파지토리를 복사해오기

```
git clone https://github.com/alexparkjw/typing.git
```
- 명령을 하면 typing 디렉토리가 생성되고, 그안에 .git/ 서브 디렉토리가 생성된다. 

- 이것을 이용하여 여러기기나 혹은 여러명이서 작업을 하고 각자 수정하여 올릴 수 있으며,마스터는 그 수정본들을 비교분석하여 선택적으로 우수한것을 택하여, 버전업하여 올리는 형식이란 것을 짐작 할 수 있다.



# 4. 기타 명령어 정리


```
git status
```
- 깃의 상태를 확인한다. 

```
git add [file]
```
- 작업디렉토리의 파일을 스태이에 올려놓는다. 

```
git checkout --[file]
```
- 스태이징의 파일을 작업공간으로 내려놓는다.

```
git commit -m "MSG"
```
- 스태이징 파일들을 로컬 리파지토리에 올려 놓는다. 

```
git push
```
- 로컬 리파지토리 올려진 파일들을 깃허브 리파지토리로 올린다.

```
git fetch
```
- 깃허브 리파지토리의 파일을 로컬 리파지토리에 내려놓는다. 

```
git merge
```
- 로컬 리파지토리의 파일을 작업디렉토리로 가져온다.

```
git log
```
- 깃허브 리파지토리의 로그를 본다.

```
git reset --hard [hash]
```
- 해시 리파지토리의 돌려놓는다.


```
git branch 
```
- master 와 추가브랜치 사이에 명령어.....



나머지는 공식메뉴얼 한글버전 https://git-scm.com/book/ko/v2 각자 터득하기 바란다..


# 5.GitHub를 활용한 프로젝트 예

```
git clone https://github.com/alexparkjw/typing.git
```
리눅스,맥 기반읜 한메타자 같은 프로그램을 GitHub 를 활용하여 프로젝트 해볼까 한다.



# 6. SSH 키 설정 (HTTPS 대신 SSH 인증)

GitHub 에 push 할 때 매번 아이디/패스워드를 입력하지 않으려면 SSH 키를 등록한다.

```
ssh-keygen -t ed25519 -C "이메일@example.com"
```
- `~/.ssh/id_ed25519` (개인키) 와 `~/.ssh/id_ed25519.pub` (공개키) 가 생성된다.

```
cat ~/.ssh/id_ed25519.pub
```
- 출력된 내용을 복사한다.
- GitHub → Settings → SSH and GPG keys → New SSH key 에 붙여넣기 한다.

```
ssh -T git@github.com
```
- `Hi 아이디! You've successfully authenticated` 메시지가 나오면 성공.

원격 주소를 HTTPS 에서 SSH 로 바꾸기:
```
git remote set-url origin git@github.com:아이디/리파지토리.git
```



# 7. 브랜치(Branch) 활용

브랜치는 원본(main/master) 을 건드리지 않고 독립적으로 작업할 수 있는 공간이다.
기능 추가나 버그 수정을 브랜치에서 하고, 완성되면 main 에 병합하는 방식이 일반적이다.

```
git branch
```
- 현재 브랜치 목록 확인. `*` 표시가 현재 위치.

```
git checkout -b 브랜치명
```
- 새 브랜치를 만들고 바로 이동.

```
git checkout main
git merge 브랜치명
```
- main 으로 이동 후 작업한 브랜치를 병합.

```
git branch -d 브랜치명
```
- 병합 완료된 브랜치 삭제.

```
git push origin 브랜치명
```
- 브랜치를 GitHub 에 올리기. (pull request 생성 가능)



# 8. 실수했을 때 되돌리기

```
git restore --staged 파일명
```
- `git add` 한 것을 취소. 파일 내용은 그대로 유지.

```
git restore 파일명
```
- 파일을 마지막 커밋 상태로 되돌리기. **주의: 수정 내용이 사라진다.**

```
git commit --amend -m "수정된 메시지"
```
- 가장 최근 커밋 메시지 수정. push 하기 전에만 사용할 것.

```
git revert 커밋해시
```
- 특정 커밋을 취소하는 새 커밋을 만든다. 이력이 보존된다.

```
git log --oneline
```
- 커밋 해시와 메시지를 한 줄로 확인. 해시 앞 7자리만 써도 된다.



# 9. 협업할 때 기본 흐름

여러 명이 같은 레포에서 작업할 때 충돌을 줄이는 순서:

```
git pull origin main        # 작업 전 최신 내용 가져오기
git checkout -b 기능명      # 새 브랜치에서 작업
git add .
git commit -m "설명"
git push origin 기능명      # GitHub 에 올리기
                            # → GitHub 에서 Pull Request 생성
                            # → 리뷰 후 main 에 merge
```

- `git pull` 을 자주 해서 최신 상태를 유지하면 충돌(conflict) 을 줄일 수 있다.
- 충돌이 생기면 해당 파일을 열어 `<<<<<<<`, `=======`, `>>>>>>>` 표시 부분을 직접 수정 후 다시 커밋하면 된다.



# 10. .gitignore

커밋하고 싶지 않은 파일은 `.gitignore` 파일에 등록한다.
빌드 결과물, 환경 설정 파일, 개인 정보 등이 해당된다.

```
# 예시 .gitignore

*.out           # 컴파일 결과물
*.o             # 오브젝트 파일
.DS_Store       # macOS 시스템 파일
option.conf     # 개인 설정 파일
.env            # 환경 변수 (비밀번호 등)
.build/         # Swift 빌드 디렉토리
```

이미 커밋된 파일은 `.gitignore` 에 추가해도 추적이 계속된다. 아래 명령어로 제거:
```
git rm --cached 파일명
```



# 11. 자주 쓰는 명령어 한눈에 보기

| 명령어 | 설명 |
|--------|------|
| `git status` | 현재 상태 확인 |
| `git diff` | 변경 내용 상세 확인 |
| `git add .` | 전체 파일 스테이징 |
| `git commit -m "msg"` | 커밋 |
| `git push origin main` | GitHub 에 올리기 |
| `git pull origin main` | GitHub 에서 가져오기 |
| `git log --oneline` | 커밋 이력 간략히 보기 |
| `git clone URL` | 레포 복사 |
| `git checkout -b 브랜치` | 새 브랜치 만들고 이동 |
| `git merge 브랜치` | 브랜치 병합 |
| `git stash` | 작업 중인 내용 임시 저장 |
| `git stash pop` | 임시 저장 내용 복원 |

공식 문서 한글판: https://git-scm.com/book/ko/v2
