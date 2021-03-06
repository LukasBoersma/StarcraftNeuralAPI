#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing damage types.</summary>
  ///
  /// @see DamageType
  ///
  /// [View on Liquipedia](http://wiki.teamliquid.net/starcraft/Damage_Type)<br>
  /// [View on Starcraft Campendium (Official Website)](http://classic.battle.net/scc/gs/damage.shtml)<br>
  /// [View on Starcraft Wikia](http://starcraft.wikia.com/wiki/Damage_types)<br>
  namespace DamageTypes
  {
    /// <summary>Enumeration of damage types.</summary>
    /// @see DamageType
    namespace Enum
    {
      /// <summary>Enumeration of damage types.</summary>
      /// @see DamageType
      enum Enum
      {
        Independent,
        Explosive,
        Concussive,
        Normal,
        Ignore_Armor,
        None,
        Unknown,
        MAX
      };
    }
  }
  /// <summary>Damage types are used in Broodwar to determine the amount of damage that will be
  /// done to a unit.</summary> This corresponds with UnitSizeType to determine the damage done to
  /// a unit.
  /// 
  /// @see WeaponType, DamageTypes, UnitSizeType
  ///
  /// [View on Liquipedia](http://wiki.teamliquid.net/starcraft/Damage_Type)<br>
  /// [View on Starcraft Campendium (Official Website)](http://classic.battle.net/scc/gs/damage.shtml)<br>
  /// [View on Starcraft Wikia](http://starcraft.wikia.com/wiki/Damage_types)<br>
  ///
  /// @ingroup TypeClasses
  class DamageType : public Type<DamageType, DamageTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      DamageType(int id = DamageTypes::Enum::None);
  };

  /// @ingroup Types
  namespace DamageTypes
  {
    /// <summary>Retrieves the set of all the DamageTypes.</summary>
    ///
    /// @returns Set of DamageTypes.
    const DamageType::set& allDamageTypes();

    extern const DamageType Independent;
    extern const DamageType Explosive;
    extern const DamageType Concussive;
    extern const DamageType Normal;
    extern const DamageType Ignore_Armor;
    extern const DamageType None;
    extern const DamageType Unknown;
  }

  static_assert(sizeof(DamageType) == sizeof(int), "Expected type to resolve to primitive size.");
}
