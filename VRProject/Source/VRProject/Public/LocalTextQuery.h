#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LocalTextQuery.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FLocalTextQueryEvent,
    const FString&, Status,
    const FString&, Reply,
    const FString&, Error
);

UCLASS()
class VRPROJECT_API ULocalTextQuery : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FLocalTextQueryEvent Completed;

    UPROPERTY(BlueprintAssignable)
    FLocalTextQueryEvent Failed;

    UFUNCTION(BlueprintCallable, Category = "Local Server",
        meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
    static ULocalTextQuery* SendLocalTextQuery(
        UObject* WorldContextObject,
        const FString& Text
    );

    virtual void Activate() override;

private:
    FString QueryText;
    bool bReceivedCompletion = false;

    void Fail(const FString& Error);
};
