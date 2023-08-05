# MaxsuPoise Mechanics Manual

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
3. **WeaponDamageMult**: If the weapon form has keyword named _MaxsuPoise_UniqueWeapStagger_, read the stagger ratio in the weapon form. If the keyword not existed, read the value that defined in "MaxsuPoise.ini".
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

1. **KeywordImmuneLevel**：The hit target's immunity to different levels of stagger is set through special keywords:

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

1. **TotalPoiseHealth**：受击者的总韧性生命值,具体计算方式看上文

2. **CurrentPoiseHealth**: 通过 BDI 注入的 graphVairbaleFloat 值，表示受击者当前剩余的韧性生命值
3. **PoiseDamage**: 攻击者单次攻击造成的韧性伤害具体计算方式看上文
4. **SmallStaggerTHLD**: MaxsuPoise.ini 上设置的全局统一数值，表示受击者触发小硬直动画的阈值
5. **MediumStaggerTHLD**: MaxsuPoise.ini 上设置的全局统一数值，表示受击者触发中硬直动画的阈值
6. **LargeStaggerTHLD**: MaxsuPoise.ini 上设置的全局统一数值，表示受击者触发大硬直动画的阈值
7. **TotalImmuneLevel**: 受击者所能免疫硬直动画的等级，具体计算方式看上文

---

<br/>

## Poise Health Regen

---

<br/>

## Infinite StaggerLock Prevention

---

<br/>
