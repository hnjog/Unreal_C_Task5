## 블로그
관련 TIL 주소 : https://hnjog.github.io/unreal/Week4Task5/

## 구현 기능
<img width="592" height="628" alt="Image" src="https://github.com/user-attachments/assets/34bf1b95-1c2f-4bf9-b519-57c74482095f" /><br>

- actor가 spawn되는 시점에 동작되도록 코드 구현<br>

---

### 클래스 설명과 기타 파일

- 클래스

```
MyActor
 - 과제에서 요구하는 지점 이동, 로깅, 이벤트 체크 등에 대한 구현
```

- 유틸 파일

```
LogTests
- 커스텀 Log 인 LogCoord 생성

MyUtils
- namespace와 ForceInline으로 묶은 기타 유틸리티 함수 묶음 파일
```

## 트러블 슈팅 - Utility 파일의 중복 선언?
<img width="2125" height="278" alt="Image" src="https://github.com/user-attachments/assets/9fc4b302-2145-481a-b62e-37c8f4cd0952" /><br>

이상하게도 MyUtils에서<br>
추가한 유틸용 함수에 '재정의'문제가 발생하였다고 한다<br>

<img width="548" height="155" alt="Image" src="https://github.com/user-attachments/assets/9f43f5a4-708e-417f-ab06-2cfe8b3ed9a4" /><br>

흐음... 딱 보니 FORCEINLINE을 빼뜨려서 그런듯한데<br>
(사실 이전 코드는 0~100의 RandomRange를 돌려서<br>
FORCEINLINE을 사용하지 않았었다)<br>

<img width="1432" height="773" alt="Image" src="https://github.com/user-attachments/assets/e166b4a6-333a-4e12-b02d-e8e6c3cd1ee4" /><br>

FORCEINLINE을 넣으니 문제 없이 컴파일 되었다?<br>

애초에 이건 왜 발생한 문제일까?<br>

### LNK 2005
'기호'(심벌)이 이미 '정의'된 경우 발생하는 링킹 에러<br>
(전처리->컴파일->어셈블->링킹 중 링킹 단계에서 발생)<br>
(일반적으로 '재정의' 관련 에러이다)<br>

- 링커가 '여러' obj나 lib 를 합치며 '같은 이름'의 '함수/전역변수' 발견시<br>
  뱉어내는 오류<br>

???<br>
이게 왜 발생?<br>
나는 분명 저 함수를 한번만 썼는데?<br>

대략적인 흐름(정확하지 않을 수 있음)<br>

- 전역 함수 (myUtils.h) 존재<br>
- 그런데 사용하는 myactor.cpp에서 include 하면서<br>
  전처리가 '이를 복붙'함<br>

- 나중에 링커가 봤는데<br>
  - 어라? myUtils.h랑 myactor.cpp에서 둘다 같은 함수 이름을 사용하네?<br>
  - lnk 2005 퉷!<br>
  (참고로 다른 h에서 include해도 lnk2005가 발생하는 것은 여전한<br>
  '같은 컴파일 단위'에서 중복이 발견될때 컴파일러가 잡아줄 수 있음)<br>

### FORCEINLINE이 해결하는 이유?

ODR(단일 정의 규칙) : 하나의 함수/변수 는 프로그램에 한번 정의되어야 함<br>
(inline 변수는 C++17에 등장)<br>

inline 속성은 해당 조건에 대한 '특례'로서 중복 선언을 허용한다<br>

- inline 함수?<br>
  : '함수 정의를 호출 지점에 그대로 삽입하는 것을 컴파일러에게 제안'하도록<br>
	설정하는 옵션<br>
	(오버헤드 감소)<br>
	(다만, 반드시 inline 되지는 않음을 유의할 것)<br>

- inline 변수(C++ 17)<br>
  : 여러 파일에서 #include 한 변수가 중복 정의되지 않도록 링커에게 알려주는 역할<br>
    특정 헤더에서 선언한 전역 변수 등을 중복 선언하지 않도록 설정<br>
	(inline 선언을 추가하면 클래스 / 구조체의<br>
	 static 정적 멤버를 cpp에서 재정의 하지 않아도 된다)<br>

 
### LNK 2005 를 피하려면

| 패턴                                | 위치           | 링크 에러?    | 비고                   |
| --------------------------------- | ------------ | --------- | -------------------- |
| 전역 함수, `inline` 없음            | 헤더(여러 TU 포함) | ✅ LNK2005 | 중복 **정의** 발생         |
| 전역 함수, `inline`/`FORCEINLINE` | 헤더           | ❌         | ODR 특례로 허용           |
| 멤버 함수, **클래스 본문 안 정의**            | 헤더           | ❌         | **암묵적 inline**       |
| 멤버 함수, 클래스 밖 정의(`inline` 없음)      | 헤더           | ✅ LNK2005 | 바깥 정의는 **inline 필요** |

- 암묵적으로 클래스 내부의 멤버 함수는 inline 함수 취급하기에 에러를 내뱉지 않음!<br>
 (그래서 Getter/Setter를 헤더에서 구현하더라도 에러가 없다!)<br>