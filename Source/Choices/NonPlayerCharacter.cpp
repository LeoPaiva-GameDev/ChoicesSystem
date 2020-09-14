// Code by Leonardo Paiva

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "ChoicesCharacter.h"
#include "Kismet/GameplayStatics.h"
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
	if (OtherActor->IsA<AChoicesCharacter>())
	{
		Character = Cast<AChoicesCharacter>(OtherActor);
		Character->SetTalkRangeStatus(true);
		Character->GeneratePlayerLines(*PlayerSpeechChoice);
		Character->SetAssociatedNPC(this);
	}
}

void ANonPlayerCharacter::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AChoicesCharacter>())
	{
		Character = Cast<AChoicesCharacter>(OtherActor);
		Character->SetTalkRangeStatus(false);
		Character->SetAssociatedNPC(nullptr);
	}
}

void  ANonPlayerCharacter::Talk(USoundBase* SpeechSound, TArray<FSubtitle> Subtitles)
{
	Character = Cast<AChoicesCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	//Play the corresponding sfx
	AudioComponent->SetSound(SpeechSound);
	AudioComponent->Play();
 
	//Tell the UI to update with the new subtitles
	Character->GetUI()->UpdateSubtitles(Subtitles);
}

void ANonPlayerCharacter::AnswerToPlayer(FName PlayerLine, TArray<FSubtitle>& SubtitlesToDisplay, float Delay)
{
	if (!NPCLines) return;
 
	//Retrieve the corresponding line
	FString ContextString;
	FDialog* Dialog = NPCLines->FindRow<FDialog>(PlayerLine, ContextString);
 
	Character = Cast<AChoicesCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
 
	if (Dialog && Character)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
 
		TimerDel.BindUFunction(this, FName("Talk"), Dialog->SFX, Dialog->Subtitles);
 
		//Talk to the player after the delay time has passed
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, Delay, false);
	}
}
