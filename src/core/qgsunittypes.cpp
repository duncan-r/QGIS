/***************************************************************************
                         qgsunittypes.cpp
                         --------------
    begin                : February 2016
    copyright            : (C) 2016 by Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsunittypes.h"
#include <QCoreApplication>

/***************************************************************************
 * This class is considered CRITICAL and any change MUST be accompanied with
 * full unit tests in test_qgsunittypes.py.
 * See details in QEP #17
 ****************************************************************************/

QgsUnitTypes::DistanceUnitType QgsUnitTypes::unitType( QGis::UnitType unit )
{
  switch ( unit )
  {
    case QGis::Meters:
    case QGis::Feet:
    case QGis::NauticalMiles:
      return Standard;

    case QGis::Degrees:
      return Geographic;

    case QGis::UnknownUnit:
      return UnknownType;
  }
  return UnknownType;
}

QgsUnitTypes::DistanceUnitType QgsUnitTypes::unitType( QgsUnitTypes::AreaUnit unit )
{
  switch ( unit )
  {
    case SquareMeters:
    case SquareKilometers:
    case SquareFeet:
    case SquareYards:
    case SquareMiles:
    case Hectares:
    case Acres:
      return Standard;

    case SquareDegrees:
      return Geographic;

    case UnknownAreaUnit:
      return UnknownType;
  }

  return UnknownType;
}

QString QgsUnitTypes::encodeUnit( QGis::UnitType unit )
{
  switch ( unit )
  {
    case QGis::Meters:
      return "meters";

    case QGis::Feet:
      return "feet";

    case QGis::Degrees:
      return "degrees";

    case QGis::UnknownUnit:
      return "<unknown>";

    case QGis::NauticalMiles:
      return "nautical miles";
  }
  return QString();
}

/***************************************************************************
 * This class is considered CRITICAL and any change MUST be accompanied with
 * full unit tests in test_qgsunittypes.py.
 * See details in QEP #17
 ****************************************************************************/

QGis::UnitType QgsUnitTypes::decodeDistanceUnit( const QString& string, bool* ok )
{
  QString normalized = string.trimmed().toLower();

  if ( ok )
    *ok = true;

  if ( normalized == encodeUnit( QGis::Meters ) )
    return QGis::Meters;
  if ( normalized == encodeUnit( QGis::Feet ) )
    return QGis::Feet;
  if ( normalized == encodeUnit( QGis::Degrees ) )
    return QGis::Degrees;
  if ( normalized == encodeUnit( QGis::NauticalMiles ) )
    return QGis::NauticalMiles;
  if ( normalized == encodeUnit( QGis::UnknownUnit ) )
    return QGis::UnknownUnit;

  if ( ok )
    *ok = false;

  return QGis::UnknownUnit;
}

QString QgsUnitTypes::toString( QGis::UnitType unit )
{
  switch ( unit )
  {
    case QGis::Meters:
      return QCoreApplication::translate( "QGis::UnitType", "meters" );
    case QGis::Feet:
      return QCoreApplication::translate( "QGis::UnitType", "feet" );

    case QGis::Degrees:
      return QCoreApplication::translate( "QGis::UnitType", "degrees" );

    case QGis::UnknownUnit:
      return QCoreApplication::translate( "QGis::UnitType", "<unknown>" );

    case QGis::NauticalMiles:
      return QCoreApplication::translate( "QGis::UnitType", "nautical miles" );
  }
  return QString();
}

/***************************************************************************
 * This class is considered CRITICAL and any change MUST be accompanied with
 * full unit tests in test_qgsunittypes.py.
 * See details in QEP #17
 ****************************************************************************/

QGis::UnitType QgsUnitTypes::stringToDistanceUnit( const QString& string, bool* ok )
{
  QString normalized = string.trimmed().toLower();

  if ( ok )
    *ok = true;

  if ( normalized == toString( QGis::Meters ) )
    return QGis::Meters;
  if ( normalized == toString( QGis::Feet ) )
    return QGis::Feet;
  if ( normalized == toString( QGis::Degrees ) )
    return QGis::Degrees;
  if ( normalized == toString( QGis::NauticalMiles ) )
    return QGis::NauticalMiles;
  if ( normalized == toString( QGis::UnknownUnit ) )
    return QGis::UnknownUnit;

  if ( ok )
    *ok = false;

  return QGis::UnknownUnit;
}

/***************************************************************************
 * This class is considered CRITICAL and any change MUST be accompanied with
 * full unit tests in test_qgsunittypes.py.
 * See details in QEP #17
 ****************************************************************************/

double QgsUnitTypes::fromUnitToUnitFactor( QGis::UnitType fromUnit, QGis::UnitType toUnit )
{
#define DEGREE_TO_METER 111319.49079327358
#define FEET_TO_METER 0.3048
#define NMILE_TO_METER 1852.0

  // Unify degree units
  // remove for QGIS 3.0, as extra degree types will be removed
  if ( fromUnit == QGis::DecimalDegrees || fromUnit == QGis::DegreesMinutesSeconds || fromUnit == QGis::DegreesDecimalMinutes )
    fromUnit = QGis::Degrees;
  if ( toUnit == QGis::DecimalDegrees || toUnit == QGis::DegreesMinutesSeconds || toUnit == QGis::DegreesDecimalMinutes )
    toUnit = QGis::Degrees;

  // Calculate the conversion factor between the specified units
  if ( fromUnit != toUnit )
  {
    switch ( fromUnit )
    {
      case QGis::Meters:
      {
        if ( toUnit == QGis::Feet ) return 1.0 / FEET_TO_METER;
        if ( toUnit == QGis::Degrees ) return 1.0 / DEGREE_TO_METER;
        if ( toUnit == QGis::NauticalMiles ) return 1.0 / NMILE_TO_METER;
        break;
      }
      case QGis::Feet:
      {
        if ( toUnit == QGis::Meters ) return FEET_TO_METER;
        if ( toUnit == QGis::Degrees ) return FEET_TO_METER / DEGREE_TO_METER;
        if ( toUnit == QGis::NauticalMiles ) return FEET_TO_METER / NMILE_TO_METER;
        break;
      }
      case QGis::Degrees:
      {
        if ( toUnit == QGis::Meters ) return DEGREE_TO_METER;
        if ( toUnit == QGis::Feet ) return DEGREE_TO_METER / FEET_TO_METER;
        if ( toUnit == QGis::NauticalMiles ) return DEGREE_TO_METER / NMILE_TO_METER;
        break;
      }
      case QGis::NauticalMiles:
      {
        if ( toUnit == QGis::Meters ) return NMILE_TO_METER;
        if ( toUnit == QGis::Feet ) return NMILE_TO_METER / FEET_TO_METER;
        if ( toUnit == QGis::Degrees ) return NMILE_TO_METER / DEGREE_TO_METER;
        break;
      }
      case QGis::UnknownUnit:
        break;
    }
  }
  return 1.0;
}

QString QgsUnitTypes::encodeUnit( QgsUnitTypes::AreaUnit unit )
{
  switch ( unit )
  {
    case SquareMeters:
      return "m2";
    case SquareKilometers:
      return "km2";
    case SquareFeet:
      return "ft2";
    case SquareYards:
      return "y2";
    case SquareMiles:
      return "mi2";
    case Hectares:
      return "ha";
    case Acres:
      return "ac";
    case SquareDegrees:
      return "deg2";
    case UnknownAreaUnit:
      return "<unknown>";
  }
  return QString();
}

QgsUnitTypes::AreaUnit QgsUnitTypes::decodeAreaUnit( const QString& string, bool* ok )
{
  QString normalized = string.trimmed().toLower();

  if ( ok )
    *ok = true;

  if ( normalized == encodeUnit( SquareMeters ) )
    return SquareMeters;
  if ( normalized == encodeUnit( SquareKilometers ) )
    return SquareKilometers;
  if ( normalized == encodeUnit( SquareFeet ) )
    return SquareFeet;
  if ( normalized == encodeUnit( SquareYards ) )
    return SquareYards;
  if ( normalized == encodeUnit( SquareMiles ) )
    return SquareMiles;
  if ( normalized == encodeUnit( Hectares ) )
    return Hectares;
  if ( normalized == encodeUnit( Acres ) )
    return Acres;
  if ( normalized == encodeUnit( SquareMiles ) )
    return SquareMiles;
  if ( normalized == encodeUnit( SquareDegrees ) )
    return SquareDegrees;
  if ( normalized == encodeUnit( UnknownAreaUnit ) )
    return UnknownAreaUnit;

  if ( ok )
    *ok = false;

  return UnknownAreaUnit;
}

QString QgsUnitTypes::toString( QgsUnitTypes::AreaUnit unit )
{
  switch ( unit )
  {
    case SquareMeters:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "square meters" );
    case SquareKilometers:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "square kilometers" );
    case SquareFeet:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "square feet" );
    case SquareYards:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "square yards" );
    case SquareMiles:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "square miles" );
    case Hectares:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "hectares" );
    case Acres:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "acres" );
    case SquareDegrees:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "square degrees" );
    case UnknownAreaUnit:
      return QCoreApplication::translate( "QgsUnitTypes::AreaUnit", "<unknown>" );
  }
  return QString();
}

QgsUnitTypes::AreaUnit QgsUnitTypes::stringToAreaUnit( const QString& string, bool* ok )
{
  QString normalized = string.trimmed().toLower();

  if ( ok )
    *ok = true;

  if ( normalized == toString( SquareMeters ) )
    return SquareMeters;
  if ( normalized == toString( SquareKilometers ) )
    return SquareKilometers;
  if ( normalized == toString( SquareFeet ) )
    return SquareFeet;
  if ( normalized == toString( SquareYards ) )
    return SquareYards;
  if ( normalized == toString( SquareMiles ) )
    return SquareMiles;
  if ( normalized == toString( Hectares ) )
    return Hectares;
  if ( normalized == toString( Acres ) )
    return Acres;
  if ( normalized == toString( SquareDegrees ) )
    return SquareDegrees;
  if ( normalized == toString( UnknownAreaUnit ) )
    return UnknownAreaUnit;
  if ( ok )
    *ok = false;

  return UnknownAreaUnit;
}

double QgsUnitTypes::fromUnitToUnitFactor( QgsUnitTypes::AreaUnit fromUnit, QgsUnitTypes::AreaUnit toUnit )
{
#define KM2_TO_M2 1000000.0
#define FT2_TO_M2 0.09290304
#define YD2_TO_M2 0.83612736
#define MI2_TO_M2 2589988.110336
#define HA_TO_M2 10000.0
#define AC_TO_FT2 43560.0
#define DEG2_TO_M2 12392029030.5

  // Calculate the conversion factor between the specified units
  if ( fromUnit != toUnit )
  {
    switch ( fromUnit )
    {
      case SquareMeters:
      {
        if ( toUnit == SquareKilometers ) return 1.0 / KM2_TO_M2;
        if ( toUnit == SquareFeet ) return 1.0 / FT2_TO_M2;
        if ( toUnit == SquareYards ) return 1.0 / YD2_TO_M2;
        if ( toUnit == SquareMiles ) return 1.0 / MI2_TO_M2;
        if ( toUnit == Hectares ) return 1.0 / HA_TO_M2;
        if ( toUnit == Acres ) return 1.0 / AC_TO_FT2 / FT2_TO_M2;
        if ( toUnit == SquareDegrees ) return 1.0 / DEG2_TO_M2;

        break;
      }
      case SquareKilometers:
      {
        if ( toUnit == SquareMeters ) return KM2_TO_M2;
        if ( toUnit == SquareFeet ) return KM2_TO_M2 / FT2_TO_M2 ;
        if ( toUnit == SquareYards ) return KM2_TO_M2 / YD2_TO_M2;
        if ( toUnit == SquareMiles ) return KM2_TO_M2 / MI2_TO_M2;
        if ( toUnit == Hectares ) return KM2_TO_M2 / HA_TO_M2;
        if ( toUnit == Acres ) return KM2_TO_M2 / AC_TO_FT2 / FT2_TO_M2 ;
        if ( toUnit == SquareDegrees ) return KM2_TO_M2 / DEG2_TO_M2;

        break;
      }
      case SquareFeet:
      {
        if ( toUnit == SquareMeters ) return FT2_TO_M2;
        if ( toUnit == SquareKilometers ) return FT2_TO_M2 / KM2_TO_M2;
        if ( toUnit == SquareYards ) return FT2_TO_M2 / YD2_TO_M2;
        if ( toUnit == SquareMiles ) return FT2_TO_M2 / MI2_TO_M2;
        if ( toUnit == Hectares ) return FT2_TO_M2 / HA_TO_M2;
        if ( toUnit == Acres ) return 1.0 / AC_TO_FT2;
        if ( toUnit == SquareDegrees ) return FT2_TO_M2 / DEG2_TO_M2;

        break;
      }

      case SquareYards:
      {
        if ( toUnit == SquareMeters ) return YD2_TO_M2;
        if ( toUnit == SquareKilometers ) return YD2_TO_M2 / KM2_TO_M2;
        if ( toUnit == SquareFeet ) return YD2_TO_M2 / FT2_TO_M2;
        if ( toUnit == SquareMiles ) return YD2_TO_M2 / MI2_TO_M2;
        if ( toUnit == Hectares ) return YD2_TO_M2 / HA_TO_M2;
        if ( toUnit == Acres ) return YD2_TO_M2 / FT2_TO_M2 / AC_TO_FT2;
        if ( toUnit == SquareDegrees ) return YD2_TO_M2 / DEG2_TO_M2;
        break;
      }

      case SquareMiles:
      {
        if ( toUnit == SquareMeters ) return MI2_TO_M2;
        if ( toUnit == SquareKilometers ) return MI2_TO_M2 / KM2_TO_M2;
        if ( toUnit == SquareFeet ) return MI2_TO_M2 / FT2_TO_M2;
        if ( toUnit == SquareYards ) return MI2_TO_M2 / YD2_TO_M2;
        if ( toUnit == Hectares ) return MI2_TO_M2 / HA_TO_M2;
        if ( toUnit == Acres ) return MI2_TO_M2 / FT2_TO_M2 / AC_TO_FT2;
        if ( toUnit == SquareDegrees ) return MI2_TO_M2 / DEG2_TO_M2;
        break;
      }

      case Hectares:
      {
        if ( toUnit == SquareMeters ) return HA_TO_M2;
        if ( toUnit == SquareKilometers ) return HA_TO_M2 / KM2_TO_M2;
        if ( toUnit == SquareFeet ) return HA_TO_M2 / FT2_TO_M2;
        if ( toUnit == SquareYards ) return HA_TO_M2 / YD2_TO_M2;
        if ( toUnit == SquareMiles ) return HA_TO_M2 / MI2_TO_M2;
        if ( toUnit == Acres ) return HA_TO_M2 / FT2_TO_M2 / AC_TO_FT2;
        if ( toUnit == SquareDegrees ) return HA_TO_M2 / DEG2_TO_M2;
        break;
      }

      case Acres:
      {
        if ( toUnit == SquareMeters ) return AC_TO_FT2 * FT2_TO_M2;
        if ( toUnit == SquareKilometers ) return AC_TO_FT2 * FT2_TO_M2 / KM2_TO_M2;
        if ( toUnit == SquareFeet ) return AC_TO_FT2;
        if ( toUnit == SquareYards ) return AC_TO_FT2 * FT2_TO_M2 / YD2_TO_M2;
        if ( toUnit == SquareMiles ) return AC_TO_FT2 * FT2_TO_M2 / MI2_TO_M2;
        if ( toUnit == Hectares ) return AC_TO_FT2 * FT2_TO_M2 / HA_TO_M2;
        if ( toUnit == SquareDegrees ) return AC_TO_FT2 * FT2_TO_M2 / DEG2_TO_M2;
        break;
      }

      case SquareDegrees:
      {
        if ( toUnit == SquareMeters ) return DEG2_TO_M2;
        if ( toUnit == SquareKilometers ) return DEG2_TO_M2 / KM2_TO_M2;
        if ( toUnit == SquareFeet ) return DEG2_TO_M2 / FT2_TO_M2;
        if ( toUnit == SquareYards ) return DEG2_TO_M2 / YD2_TO_M2;
        if ( toUnit == SquareMiles ) return DEG2_TO_M2 / MI2_TO_M2;
        if ( toUnit == Hectares ) return DEG2_TO_M2 / HA_TO_M2;
        if ( toUnit == Acres ) return DEG2_TO_M2 / FT2_TO_M2 / AC_TO_FT2;
        break;
      }

      case UnknownAreaUnit:
        break;
    }
  }
  return 1.0;
}

// enable for QGIS 3.0
#if 0

QString QgsUnitTypes::encodeUnit( QgsSymbolV2::OutputUnit unit )
{
  switch ( unit )
  {
    case QgsSymbolV2::MM:
      return "MM";
    case QgsSymbolV2::MapUnit:
      return "MapUnit";
    case QgsSymbolV2::Pixel:
      return "Pixel";
    case QgsSymbolV2::Percentage:
      return "Percentage";
    default:
      return "MM";
  }
}

QgsSymbolV2::OutputUnit QgsUnitTypes::decodeSymbolUnit( const QString& string, bool* ok )
{
  QString normalized = string.trimmed().toLower();

  if ( ok )
    *ok = true;

  if ( normalized == encodeUnit( QgsSymbolV2::MM ).toLower() )
    return QgsSymbolV2::MM;
  if ( normalized == encodeUnit( QgsSymbolV2::MapUnit ).toLower() )
    return QgsSymbolV2::MapUnit;
  if ( normalized == encodeUnit( QgsSymbolV2::Pixel ).toLower() )
    return QgsSymbolV2::Pixel;
  if ( normalized == encodeUnit( QgsSymbolV2::Percentage ).toLower() )
    return QgsSymbolV2::Percentage;

  if ( ok )
    *ok = false;

  // millimeters are default
  return QgsSymbolV2::MM;
}

#endif