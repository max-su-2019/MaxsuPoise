# MaxsuPoise Mechanics Manual

This document would introduce you details about the mechanics of MaxsuPosie.

## Table of contents

- [**Calculating Total Poise Health**](#Calculating-Total-Poise-Health)
- [**Calculating Poise Damage**](#Calculating-Poise-Damage)
- [**Calculating Total Immune Level**](#Calculating-Total-Immune-Level)
- [**Trigger Stagger**](#Trigger-Stagger)
- [**Poise Health Regen**](#Poise-Health-Regen)
- [**Infinite StaggerLock Prevention**](#Infinite-StaggerLock-Prevention)

<br/>

## Calculating Total Poise Health

### List of Variables:

1. **BasePoiseHealth**: Global value that defined in "MaxsuPoise.ini", repersents the base poise health of the hit target.

2. **BaseMass**: Value defined in the race form, vary for actor of different races, represents the base mass value of the hit target.
3. **Scale**: A standalone value for each actors, represents the body scale of the hit target.
4. **BaseArmorPoiseHealth**: Global value that defined in "MaxsuPoise.ini", repersents the extra poise health brought by an armor.
5. **HeavyArmorPoiseBonus**: Global value that defined in "MaxsuPoise.ini", represents the extra poise health bonus ratio when the armor is heavy armor.

### Formula:

```
LightArmorHealth = BaseArmorPoiseHealth

HeavyArmorHealth =  LightArmorHealth * (1 + HeavyArmorPoiseBonus)

TotalPoiseHealth = BasePoiseHealth * BaseMass * Scale + WornLightArmorNums * LightArmorHealth + WornHeavyArmorNums * HeavyArmorHealth
```

---

<br/>

## Calculating Poise Damage

### List of Variables:

1. **BaseMeleePoiseDamage**: Global value that defined in "MaxsuPoise.ini", represents the base melee poise damage of the attacker cause to the hit target.

2. **BaseRangePoiseDamage**: Global value that defined in "MaxsuPoise.ini", represents the base range poise damage of the attacker cause to the hit target.
3. **WeaponDamageMult**: If the weapon form has keyword named _MaxsuPoise_UniqueWeapStagger_, read from the stagger ratio defined inside the weapon form. If the keyword not existed, read from the value that defined in "MaxsuPoise.ini".
4. **AnimationDamageMult**: Poise damage mult ratio that read from the attack animation annotations of the attacker.
5. **AttackDataStagger**: Poise damage mult ratio that read from the current attack data stagger value of the attacker.
6. **BaseMagicPoiseDamage**: Global value that defined in "MaxsuPoise.ini", represents the base magic effect poise damage of the attacker cause to the hit target. **Only the magic effect with stagger archetype could deal poise damage thus stagger the hit target!**
7. **MagicMagnitude**: The magnitude value from the stagger magic effect attached to the spell form.
8. **ModTargetStagger**: The ModTargetStagger perk ratio of the attacker.
9. **ModIncomingStagger**: The ModIncomingStagger perk ratio of the hit target.
10. **BlockingMult**:
    According to the `BlockedMode` on MaxsuPoise.ini, there are two cases: If BlockedMode is set to `PercentBlocked`，the value equal to (1 - vanillaBlockedPercent). If BlockedMode is set to `FullyBlocked`, the value equal to 0.
11. **StrengthMult**: `(attacker BaseMass * attacker Scale) / (target BaseMass * target Scale)`

### Formula:

- **Physicial Damage**:

```
Melee/RangePoiseDamage = BaseMelee/RangePoiseDamage * (WeaponDamageMult + StrengthMult + AttackDataStagger) * (AnimationDamageMult + 1) * ModTargetStagger * ModIncomingStagger
```

If the hit is blocked, then `PoiseDamage *= BlockingMult`
<br/>

- **Magic Damage**:

```
  MagicPoiseDamage = BaseMagicPoiseDamage * MagicMagnitude
```

**Only works for magic effect with stagger archetype!**

---

<br/>

## Calculating Total Immune Level

### List of Variables:

1. **KeywordImmuneLevel**：The hit target`s immunity to different levels of stagger is set through special keywords:

   - MaxsuPoise_ImmuneSmall
   - MaxsuPoise_ImmuneMedium
   - MaxsuPoise_ImmuneLarge
   - MaxsuPoise_ImmuneLargest

They represent the four levels of stagger the hit target would be immune to. Immunity keywords can be added directly to the actor or by casting a magic effect with the corresponding keyword on the actor.
If there are multiple Immune keywords at the same time, the keyword with the highest Immune level will be used as the final value.

1. **AnimationImmuneLevel**: Immune Level that read from the current activated animation annotations of the hit target.

### Formula:

```
TotalImmuneLevel = MAX(KeywordImmuneLevel,AnimationImmuneLevel)
```

Get the maximum value between KeywordImmuneLevel and AnimationImmuneLevel.

---

<br/>

## Trigger Stagger

### List of Variables:

1. **TotalPoiseHealth**：See the above for the specific calculation method.

2. **CurrentPoiseHealth**: A graphVairbaleFloat value injected through BDI, indicating the current remaining poise health of the hit target.
3. **PoiseDamage**: See the above for the specific calculation method.
4. **SmallStaggerTHLD**: Global value that defined in "MaxsuPoise.ini", represents the threshold of the poise damage that could drive the hit target trigger small stagger.
5. **MediumStaggerTHLD**: Global value that defined in "MaxsuPoise.ini", represents the threshold of the poise damage that could drive the hit target trigger medium stagger.
6. **LargeStaggerTHLD**: Global value that defined in "MaxsuPoise.ini", represents the threshold of the poise damage that could drive the hit target trigger large stagger.
7. **TotalImmuneLevel**: See the above for the specific calculation method.

### Stagger Trigger Mechanics:

1. The inital value of the _CurrentPoiseHealth_ for a hit target is equal to it _TotalPoiseHealth_.

2. Once the hit target receive damage from the attacker within an attack:  
   `CurrentPoiseHealth -= PoiseDamage`
3. If `CurrentPoiseHealth <= 0`, largest level of stagger animation would be triggered on the hit target.
4. If `CurrentPoiseHealth > 0`, calculating the result of `PoiseDamage / TotalPoiseHealth`:
   - If `result < SmallStaggerTHLD`，Not stagger animation would be triggered.
   - If `SmallStaggerTHLD <= result < MediumStaggerTHLD` and `TotalImmuneLevel < 1`, small level of stagger animation would be triggered on the hit target.
   - If `MediumStaggerTHLD <= result < LargeStaggerTHLD` and `TotalImmuneLevel < 2`, medium level of stagger animation would be triggered on the hit target.
   - If `result >= LargeStaggerTHLD` and `TotalImmuneLevel < 3`, large level of stagger animation would be triggered on the hit target.

---

<br/>

## Poise Health Regen

### List of Variables:

1. **PoiseRegen**: Global value that defined in "MaxsuPoise.ini", represents the poise health regen ratio of the hit target.

### Implement Methods:

1. If `0 < CurrentPoiseHealth < TotalPoiseHealth`，for each second: `CurrentPoiseHealth += PoiseRegen`
2. If `CurrentPoiseHealth > TotalPoiseHealth` then `CurrentPoiseHealth = TotalPoiseHealth`
3. If `CurrentPoiseHealth <= 0`,
   - If hit stagger is staggering, Immune to any Poise Damage and stagger.
   - If hit stagger is not staggering，`currentPoiseHealth = TotalPoiseHealth`

---

<br/>

## Infinite StaggerLock Prevention

### List of Variables:

1. **StaggerProtectTime**: Global value that defined in "MaxsuPoise.ini", represents the stagger protect time after the hit target recovered from largest stagger state.

### Implement Methods:

- When `CurrentPoiseHealth <= 0` and the hit target is playing largest stagger animation，the actor would Immune to any Poise Damage and stagger.

- After the hit target recovered from largest stagger state, he will gained a stagger protect effect equivalents to `TotalImmuneLevel = 3`, the effect would lasting until StaggerProtectTime end.

---

<br/>
