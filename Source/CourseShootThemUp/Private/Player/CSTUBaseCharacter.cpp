// CourseShootThemUp

#include "Player/CSTUBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/CSTULauncherWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogMovement, All, All)

// Sets default values
ACSTUBaseCharacter::ACSTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCSTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UCSTUHealthComponent>("HealthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<UCSTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ACSTUBaseCharacter::BeginPlay() {
    Super::BeginPlay();
    // check components for null
    // work only in developer and debug  build, not working in shipping build
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ACSTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ACSTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ACSTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ACSTUBaseCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    // const auto Health = HealthComponent->GetHealth();
    // Disable because text is update by delegate
    // HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

// Called to bind functionality to input
void ACSTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MovementForward", this, &ACSTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MovementRight", this, &ACSTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ACSTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ACSTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACSTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACSTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ACSTUBaseCharacter::OnEndRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UCSTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UCSTUWeaponComponent::StopFire);
}

void ACSTUBaseCharacter::MoveForward(float Scale) {
    IsMovingForward = Scale > 0 ? true : false;
    if (Scale != 0.0f) {
        AddMovementInput(GetActorForwardVector(), Scale);
    }
}
void ACSTUBaseCharacter::MoveRight(float Scale) {
    if (Scale != 0.0f) {
        AddMovementInput(GetActorRightVector(), Scale);
    }
}

void ACSTUBaseCharacter::OnStartRunning() {
    WantToRun = true;
}
void ACSTUBaseCharacter::OnEndRunning() {
    WantToRun = false;
}

bool ACSTUBaseCharacter::IsRunning() const {
    return WantToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ACSTUBaseCharacter::GetMovementDirection() const {

    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

    ////Debug drawline for better understanding math
    // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetVelocity(), FColor::Red, false, 0, 0U, 5);
    // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 300, FColor::Blue, false, 0, 0U, 5);
    // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + CrossProduct * 300, FColor::Green, false, 0, 0U, 5);
    // UE_LOG(LogMovement, Display, TEXT("Degrees %lf"), Degrees);

    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ACSTUBaseCharacter::OnDeath() {
    UE_LOG(BaseCharacterLog, Display, TEXT("Character is dead"));
    PlayAnimMontage(DeathAnimMontage);
    // get character movement component and disable movement(because we are dead)
    GetCharacterMovement()->DisableMovement();
    // destoy actor after 5 sec
    SetLifeSpan(5.0f);
    if (Controller) {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ACSTUBaseCharacter::OnHealthChanged(float Health) {
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ACSTUBaseCharacter::OnGroundLanded(const FHitResult& Hit) {
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);
    if (FallVelocityZ < LandedDamageVelocity.X) return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("Final damage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
}

