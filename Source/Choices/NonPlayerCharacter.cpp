// Code by Leonardo Paiva

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "NonPlayerCharacter.h"

// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName ("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(BoxComponent);

}

// Called when the game starts or when spawned
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANonPlayerCharacter::OnBoxOverlapBegin);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ANonPlayerCharacter::OnBoxOverlapEnd);
}

void ANonPlayerCharacter::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ANonPlayerCharacter::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}



