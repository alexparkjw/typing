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

b. git repository 생성 : 웹페이지에서 new repository 클릭한후 repository name 을 지어주고create repository 버튼을 눌러서 만든다. (여기선 아이디는 alexparkjw 이고 리파지토리 이름은 ask 이라고 임으로 만들었다)

터미널에서 
```
echo "# ask" >> README.md
git init
git add README.md
git commit -m "first commit"
git remote add origin https://github.com/alexparkjw/typing.git
git push -u origin master
```

단, 본인의 아이디와, 본인이 정한 리파지토리 이름으로 바꿔줘야한다.

https://github.com/alexparkjw/typing.git 주소를 https://github.com/본인의아이디/리파지토리이름.git  이렇게 말이다.

위의 명령어는 본인의 컴퓨터 안에 작업할 디렉토리 안에서 해주면 .git/ 이라는 서브 디렉토리가 생성된다.


```
git rm -f filename
git checkout filename
```

파일을 지우고, add 파일을 수정할 수 있다. 


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
- 큰따옴표 안에는 작업 내용을 코맨트 하는 것이다. 생략하면 안된다. 이내용을 기반으로 해시 테이블에 넣어서 작업내용을 분류하기 때문이다.




e. 리파지토리에 올리는 명령어
```
git push -u origin master
```
- 마스터가 원본을 수정하고, 올린다는 뜻이다. 이 마스터 등급은 기본값이고, 여러 등급으로 나눌 수 있다...


f. 다른 작업공간에서 리파지토리를 복사해오기

```
git clone https://github.com/alexparkjw/typing.git
```

- 명령을 하면 ask 디렉토리가 생성되고, 그안에 .git/ 서브 디렉토리가 생성된다. 

이것을 이용하여 여러기기나 혹은 여러명이서 작업을 하고 각자 수정하여 올릴 수 있으며,
마스터는 그 수정본들을 비교분석하여 선택적으로 우수한것을 택하여, 버전업하여 올리는 형식이란 것을 짐작 할 수 있다.



g.나머지는 공식메뉴얼 한글버전 https://git-scm.com/book/ko/v2 각자 터득하기 바란다..


# 4.GitHub를 활용한 프로젝트 예
/typing, 리눅스 기반의 한컴타자나, 한메타자 같은 프로그램을 GitHub 를 활용하여 프로젝트 해볼까 한다.


#5 https://github.com/alexparkjw/typing.git

