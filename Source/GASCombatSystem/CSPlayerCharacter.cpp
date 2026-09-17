// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// 이거 안 넣으면 .gen.cpp 포함이 안되서 리플렉션 기능이 찐빠난다고 하는데 안넣어서 문제가 생기는 경우를 아직 못봄...
#include UE_INLINE_GENERATED_CPP_BY_NAME(CSPlayerCharacter)

// GamplayTag 초기화
UE_DEFINE_GAMEPLAY_TAG(MovingBlockTag, "Gameplay.State.MovingBlocked");
UE_DEFINE_GAMEPLAY_TAG(NowAttackingTag, "Gameplay.State.Attacking");
UE_DEFINE_GAMEPLAY_TAG(NextAttackTag, "Gameplay.State.NextAttack");

// Sets default values
ACSPlayerCharacter::ACSPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러에 맞춰서 액터가 회전하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 현재 회전 방향으로 MovementComponent를 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// 셀카봉 생성 및 설정
	// 컴포넌트를 생성하는 API 
	// UObject를 생성하는 키워드가 CreateDefaultSubobject와 NewObject 2가지 방법이있음
	// CreateDefaultSubobject-> 해당 UOjbect를 클래스(이 경우 CSCharacter를 의미)의 CDO에 포함하고 싶은 경우 사용
	// NewObject -> 런타임 중에 UObject의 인스턴스를 만들 때 일반적으로 사용
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);		// 액터의 루트 컴포넌트 하위에 셀카봉 컴포넌트를 등록
	CameraBoom->TargetArmLength = 500.f;			// 셀카봉 길이
	CameraBoom->bUsePawnControlRotation = true;		// 셀카봉이 Pawn의 회전에 맞춰서 같이 회전 할지

	// 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);		// 카메라를 셀카봉 아래에 등록(카메라는 셀카봉이랑 한몸이기 때문)
	FollowCamera->bUsePawnControlRotation = false;	// 카메라는 회전할 필요 없음. 회전은 셀카봉만

	// ASC 생성
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

// Called when the game starts or when spawned
void ACSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// GameplayAbility 등록
	if (ASC)
	{
		AttackAbilityHandles.Reserve(AttackAbilities.Num());
		for (const auto& Ability : AttackAbilities)
		{
			// 공격 Ability 등록. 현재 Ability는 SubclassOf 형태이므로 인스턴스가 아닌 Class 타입
			// 그렇기 때문에 실제 ClassObject를 가져와서 ASC에 할당해야함. 이 경우 CDO(Class Default Object)를 가져오면 됨
			UGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UGameplayAbility>();
			// 두 번째 인자는 Ability내부의 GameplayEffect의 레벨 값
			FGameplayAbilitySpec AttackAbilitySpec(AbilityCDO, 1);
			FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AttackAbilitySpec);
			if (Handle.IsValid())
			{
				AttackAbilityHandles.Emplace(Handle);
			}
		}
	}
}

// Called every frame
void ACSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ensure(InputMappingContext);

	// AI가 아니라면
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA과 실행 동작을 맵핑
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
	}

}

void ACSPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// AbilitySystemComponent의 OwnerAcotr 설정
	// GAS의 기준이 플레이어 컨트롤러인 경우는 첫번째 인자에 컨트롤러를 추가
	ASC->InitAbilityActorInfo(this, this);
}

void ACSPlayerCharacter::Move(const FInputActionValue& InValue)
{
	// Gameplay.State.MovingBlocked가 설정된 경우 이동 처리를 막음
	if (ASC->HasMatchingGameplayTag(MovingBlockTag))
	{
		return;
	}

	if (Controller == nullptr)
	{
		return;
	}

	const FVector2D MovementVector = InValue.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add input
	AddMovementInput(ForwardDir, MovementVector.Y);		// Y축 입력(W,S)을 정면으로
	AddMovementInput(RightDir, MovementVector.X);		// X축 입력(A,D)을 좌우로

}

void ACSPlayerCharacter::Look(const FInputActionValue& InValue)
{
	if (Controller == nullptr)
	{
		return;
	}

	const FVector2D LookAxisVector = InValue.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);		// 좌,우 방향에 대한 입력은 Yaw(Z축)으로 회전
	AddControllerPitchInput(LookAxisVector.Y);		// 위, 아래에 대한 입력은 Pitch(Y축)으로 회전
}

void ACSPlayerCharacter::Attack()
{
	// 현재 공격 중이라면
	if (ASC->HasMatchingGameplayTag(NowAttackingTag))
	{
		// 콤보 공격을 호출
		bCallNextAttack = true;
	}
	else
	{
		ComboIndex = 0;
		bCallNextAttack = false;
		// TryActivateAbility : 이걸로 호출하면 내부에서 이것저것 체크한 후에 Ability를 실행시킴
		bool bActivate = ASC->TryActivateAbility(AttackAbilityHandles[ComboIndex]);

		// 공격 Ability가 실행되고 다음 공격 Ability가 있다면
		if (bActivate && (ComboIndex + 1 < AttackAbilityHandles.Num()))
		{
			// 특정 GameplayTag가 등록되었을 때 발동하고 싶은 함수를 Delegate에 등록
			// Handle을 따로 관리하는 이유는 Delegate를 다 사용했으면 Remove 과정이 필요하기 때문
			ComboDelegateHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(NextAttackTag).AddUObject(this, &ThisClass::StartNextAttack);
		}
	}
}

void ACSPlayerCharacter::Jump()
{
	Super::Jump();
}

void ACSPlayerCharacter::StartNextAttack(const FGameplayEventData* InPlayload)
{
	if (bCallNextAttack == false)
	{
		return;
	}

	if (ComboIndex + 1 < AttackAbilityHandles.Num())
	{
		ComboIndex++;
		bCallNextAttack = false;
		bool bActivate = ASC->TryActivateAbility(AttackAbilityHandles[ComboIndex]);
		// 실행하지 못했다면 기존 등록했던 Delegate의 구독을 해제
		if (!bActivate)
		{
			RemoveAttackDelegate();
		}
	}
}

void ACSPlayerCharacter::RemoveAttackDelegate()
{
	if (ComboDelegateHandle.IsValid())
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(NextAttackTag).Remove(ComboDelegateHandle);
	}
}

UAbilitySystemComponent* ACSPlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACSPlayerCharacter::RemoveComboAttackBinding_Implementation()
{
	RemoveAttackDelegate();
}
