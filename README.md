# 🎮 Cop And Robber
<img width="1536" height="1024" alt="Image" src="https://github.com/user-attachments/assets/96ff86b4-0cf3-43f8-a0c9-bc52ac53436e" />
> Unreal Engine 기반 **서바이벌 액션 게임**  
> Stage 내에서 AI속에 숨은 플레이어 서로를 찾아 **최후의 1인**이 되기 위한 전투를 벌이는 게임입니다.

---

## 📘 프로젝트 개요 (Overview)
<!-- 프로젝트 전반에 대한 간단한 설명 -->
- **프로젝트명** : Cop And Robber  
- **장르** : Survival / PvP / Action  
- **엔진** : Unreal Engine 5.5  
- **플랫폼** : PC  
- **개발 기간** : 2025.09 ~ 2025.10  
- **개발 인원** : 5인 팀 프로젝트  
- **간단한 소개** :  
  동일한 외형의 AI와 플레이어들이 섞여 있는 Stage에서, 플레이어는 다른 참가자를 찾아 제거하고 최후의 1인이 되어야 합니다.

---

## 🧭 게임 플로우 (Game Flow)
<!-- 게임의 전체 진행 흐름을 시각적으로 표현 -->
```mermaid
flowchart TD
    A[게임 시작] --> B[로비 입장]
    B --> C[로비에서 다른 플레이어 대기]
    C --> D[Stage 진입]
    D --> E[플레이어 & AI 스폰]
    E --> F[탐색 및 의심 대상 식별]
    F --> G[전투 발생 - 한 번의 공격]
    G --> H{생존 여부}
    H -->|사망| I[탈락]
    H -->|생존| J{최후 1인?}
    J -->|아니오| F
    J -->|예| K[승시 로직 구현
- ✅ **AI 스포너 시스템** - 동적 AI 생성 및 관리

### 🧩 스테이지 시스템
- ✅ **멀티플레이어 지원** - 네트워크 기반 멀티플레이어 게임
- ✅ **로비 시스템** - 플레이어 대기 및 게임 시작 관리
- ✅ **게임 페이즈 관리** - 대기/진행/종료 단계별 관리

### 🎨 UI 및 시각 효과
- ✅ **실시간 게임 정보 UI** - 타이머, 생존자 수, 순위 표시
- ✅ **결과 화면** - 최종 순위 및 게임 결과 표시
- ✅ **로비 UI** - 플레이어 대기 및 준비 상태 표시

### ⚙️ 시스템 아키텍처
- ✅ **GameMode/GameState 패턴** - 중앙집중식 게임 상태 관리
- ✅ **Unreal Gameplay Ability System** - 모듈화된 어빌리티 시스템
- ✅ **네트워크 리플리케이션** - 멀티플레이어 동기화

---

## 🧩 트러블슈팅 (Trouble Shooting)
<!-- 개발 중 겪었던 문제와 해결 과정을 기록 -->

| 문제 상황 | 원인 | 해결 방법 |
|-----------|------|-----------|
| **GameplayCue 패키징 문제** | GameplayCue 경로 설정 오류 | DefaultGame.ini에서 GameplayCueNotifyPaths 경로 수정 |
| **순위 계산 동기화 문제** | 클라이언트별 순위 계산 차이 | 서버에서 중앙집중식 순위 계산 후 RPC로 전달 |
| **AI 스폰 타이밍 문제** | BeginPlay 실행 순서 문제 | Timer를 활용한 지연 스폰 처리 |
| **AttributeSet 사망 처리** | GameMode 접근 권한 문제 | World->GetAuthGameMode()를 통한 안전한 접근 |

---

## 🧰 사용한 툴 (Tools)
<!-- 프로젝트 개발 및 관리에 사용한 툴 정리 -->

| 분류 | 사용 툴 |
|------|---------|
| **Game Engine** | Unreal Engine 5.5 |
| **Language** | C++ |
| **Version Control** | Git / GitHub |
| **Graphics & Assets** | Unreal Engine Marketplace Assets |
| **IDE** | Rider |


---

## 🎥 시연 영상 (Demo Video)
🔗 [시연 영상 보러가기](https://youtu.be/<!-- TODO: 유튜브 링크 -->)

<p align="center">
  <a href="https://youtu.be/TODO">
    <img src="Docs/demo_thumbnail.gif" width="600" alt="Cop And Robber Demo Preview"/>
  </a>
</p>

---

## 🧠 후기
*
Unreal Engine의 Gameplay Ability System과 네트워킹 시스템을 깊이 있게 학습할 수 있었던 프로젝트입니다.

---

## 💬 개발자 

| 항목 | 정보 |
|------|------|
| **Developer 1** | 박준범  |
| **Email 1** | pjb990@naver.com |
| **GitHub 1** | [https://github.com/pjb990@gmail.com]|
| | |
| **Developer 2** |박진권|
| **Email 2** | wlsrnjs945@naver.com|
| **GitHub 2** | [https://github.com/jin945]|
| | |
| **Developer 3** | <!-- 이름 또는 닉네임 --> |
| **Email 3** | <!-- 이메일 주소 --> |
| **GitHub 3** | [https://github.com/<!-- 깃허브아이디 -->](https://github.com/<!-- 깃허브아이디 -->) |
| | |
| **Developer 4** | <!-- 이름 또는 닉네임 --> |
| **Email 4** | <!-- 이메일 주소 --> |
| **GitHub 4** | [https://github.com/<!-- 깃허브아이디 -->](https://github.com/<!-- 깃허브아이디 -->) |
| | |
| **Developer 5** | <!-- 이름 또는 닉네임 --> |
| **Email 5** | <!-- 이메일 주소 --> |
| **GitHub 5** | [https://github.com/<!-- 깃허브아이디 -->](https://github.com/<!-- 깃허브아이디 -->) |
