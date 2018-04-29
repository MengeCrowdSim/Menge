/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file    Attribute.h
 @brief    Defines a single attribute of an element.
 */

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Vector2.h"
#include "MengeCore/MengeException.h"

#include <string>

// forward declaration
class TiXmlElement;

namespace Menge {

namespace Math {
class FloatGenerator;
class IntGenerator;
class Vec2DGenerator;
}  // namespace Math
using namespace Math;

/*!
 @brief    Exception class for attribute definition problems (i.e., using the wrong type of
 attribute for the wrong data-type).
 */
class MENGE_API AttributeDefinitionException : public virtual Menge::MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  AttributeDefinitionException() : Menge::MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AttributeDefinitionException(const std::string& s) : Menge::MengeException(s) {}
};

/*!
 @brief    The fatal attribute definition exception.
 */
class MENGE_API AttributeDefinitionFatalException : public AttributeDefinitionException,
                                                    public Menge::MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  AttributeDefinitionFatalException()
      : Menge::MengeException(), AttributeDefinitionException(), Menge::MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AttributeDefinitionFatalException(const std::string& s)
      : Menge::MengeException(s), AttributeDefinitionException(), Menge::MengeFatalException() {}
};

/*!
 @brief    An element attribute.

 This serves as a simple mechanism to configure XML parsing for elements who only require simple
 xml-parameter-style arguments.  See AttributeSet for proper user. This is the entity responsible
 for parsing the XML and extracting the value specified.

 The Attribute also stores the parsed value and reports on its own "validity". The validity of an
 Attribute is based on whether it is set to be "required" (i.e., the user *must* explicitly specify
 a value).  If a required attribute is not defined in the XML, then it is invalid.
 */
class MENGE_API Attribute {
 public:
  /*!
   @brief    Constructor

   @param    name      The attribute name - used in parsing XML attributes.
   @param    required  Whether or not the attribute is required (true) or not (false).
   */
  Attribute(const std::string& name, bool required) : _name(name), _required(required) {}

  /*!
   @brief    Destructor
   */
  virtual ~Attribute() {}

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction.
   */
  virtual bool extract(TiXmlElement* node) = 0;

  /*!
   @brief    Returns a const reference to the name of the attribute.

   @returns  The name of the attribute.
   */
  inline const std::string& getName() const { return _name; }

  /*!
   @brief    Reports if this attribute is required to be specified.

   @returns  True if this attribute must be specified.
   */
  inline bool isRequired() const { return _required; }

  /*!
   @brief    Reports if the attribute is valid.

   @returns  True if the attribute is properly initialized.
   */
  inline bool isValid() const { return _valid; }

  /*!
   @brief    Prepares the attribute for a parse attempt.
   */
  void clear();

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() = 0;

  /*!
   @brief    Retrieve the parsed int value.

   If the type of this attribute is not an int, an exception will be thrown.

   @returns    The int value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual int getInt() {
    throw AttributeDefinitionException("This Attribute can't provide an int value.");
  }

  /*!
   @brief    Retrieve the parsed boolean value.

   If the type of this attribute is not a bool, an exception will be thrown.

   @returns    The boolean value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual bool getBool() {
    throw AttributeDefinitionException("This Attribute can't provide a boolean value.");
  }

  /*!
   @brief    Retrieve the parsed float value.

   If the type of this attribute is not a float, an exception will be thrown.

   @returns   The float value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual float getFloat() {
    throw AttributeDefinitionException("This Attribute can't provide anfloat value.");
  }

  /*!
   @brief    Retrieve the parsed string value.

   If the type of this attribute is not a string, an exception will be thrown.

   @returns   The string value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual std::string getString() {
    throw AttributeDefinitionException("This Attribute can't provide a string value.");
  }

  /*!
   @brief    Retrieve the parsed size_t value.

   If the type of this attribute is not a size_t, an exception will be thrown.

   @returns   The size_t value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual size_t getSizeT() {
    throw AttributeDefinitionException("This Attribute can't provide a size_t value.");
  }

  /*!
   @brief    Retrieve the parsed float generator.

   If the type of this attribute is not a float generator, an exception will be thrown.

   @returns    A pointer to a float generator. The float generator becomes the responsibility of the
              caller.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual FloatGenerator* getFloatGenerator() {
    throw AttributeDefinitionException("This Attribute can't provide a float generator.");
  }

  /*!
   @brief    Retrieve the parsed 2D float generator.

   @returns    A pointer to a 2D float generator.  The float generator becomes the responsibility of
              the caller.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual Vec2DGenerator* getVec2DGenerator() {
    throw AttributeDefinitionException(
        "This Attribute can't provide a 2D float "
        "generator.");
  }

  /*!
   @brief    Retrieve the parsed int generator.

   If the type of this attribute is not an int generator, an exception will be thrown.

   @returns   A pointer to a float generator.  The float generator becomes the responsibility of
              the caller.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual IntGenerator* getIntGenerator() {
    throw AttributeDefinitionException("This Attribute can't provide an int generator.");
  }

 protected:
  /*!
   @brief    The name of the xml attribute.
   */
  std::string _name;

  /*!
   @brief    Determines if the XML *must* specify this attribute.
   */
  bool _required;

  /*!
   @brief    Reports if the attribute is valid.  An attribute is always valid if it is not required.
   Otherwise, it is only valid if it was able to be initialized from the XML.
   */
  bool _valid;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a string-type value.
 */
class MENGE_API StringAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name      The attribute name.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided.
   */
  StringAttribute(const std::string& name, bool required, const std::string& defValue = "")
      : Attribute(name, required), _default(defValue) {}

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _value = _default; }

  /*!
   @brief    Retrieve the parsed string value.

   @returns    The string value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual std::string getString() { return _value; }

 protected:
  /*!
   @brief    The default string value.  Meaningless if _required is true.
   */
  std::string _default;

  /*!
   @brief    The parsed attribute value.
   */
  std::string _value;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a int-type value.
 */
class MENGE_API IntAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name      The attribute name.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided.
   */
  IntAttribute(const std::string& name, bool required, int defValue)
      : Attribute(name, required), _default(defValue) {}

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _value = _default; }

  /*!
   @brief    Retrieve the parsed int value.

   @returns    The int value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual int getInt() { return _value; }

 protected:
  /*!
   @brief    The default int value.  Meaningless if _required is true.
   */
  int _default;

  /*!
   @brief    The parsed attribute value.
   */
  int _value;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a float-type value.
 */
class MENGE_API FloatAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name      The attribute name.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided.
   */
  FloatAttribute(const std::string& name, bool required, float defValue)
      : Attribute(name, required), _default(defValue) {}

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _value = _default; }

  /*!
   @brief    Retrieve the parsed float value.

   @returns    The float value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual float getFloat() { return _value; }

 protected:
  /*!
   @brief    The default float value.  Meaningless if _required is true.
   */
  float _default;

  /*!
   @brief    The parsed attribute value.
   */
  float _value;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a boolean-type value.
 */
class MENGE_API BoolAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name    The attribute name.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided.
   */
  BoolAttribute(const std::string& name, bool required, bool defValue)
      : Attribute(name, required), _default(defValue) {}

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _value = _default; }

  /*!
   @brief    Retrieve the parsed boolean value.

   @returns    The boolean value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual bool getBool() { return _value; }

 protected:
  /*!
   @brief    The default bool value.  Meaningless if _required is true.
   */
  bool _default;

  /*!
   @brief    The parsed attribute value.
   */
  bool _value;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a size_t-type value.
 */
class MENGE_API SizeTAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name    The attribute name.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided.
   */
  SizeTAttribute(const std::string& name, bool required, size_t defValue)
      : Attribute(name, required), _default(defValue) {}

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _value = _default; }

  /*!
   @brief    Retrieve the parsed size_t value.

   @returns   The size_t value.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual size_t getSizeT() { return _value; }

 protected:
  /*!
   @brief    The default bool value.  Meaningless if _required is true.
   */
  size_t _default;

  /*!
   @brief    The parsed attribute value.
   */
  size_t _value;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a float distribution value.
 */
class MENGE_API FloatDistributionAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name      The attribute name - in this case, interpreted as the the prefix for the
                      distribution values.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided. This will be
                      interpreted as a const *distribution.
   @param    scale      The optional scale value for this float distribution.
   */
  FloatDistributionAttribute(const std::string& name, bool required, float defValue,
                             float scale = 1.f)
      : Attribute(name, required), _default(defValue), _scale(scale), _generator(0x0) {}

  /*!
   @brief    Destructor
   */
  virtual ~FloatDistributionAttribute();

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _generator = 0x0; }

  /*!
   @brief    Retrieve the parsed float generator.

   @returns    A pointer to a float generator.  The float generator becomes the responsibility of
              the caller.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual FloatGenerator* getFloatGenerator() {
    FloatGenerator* val = _generator;
    _generator = 0x0;
    return val;
  }

 protected:
  /*!
   @brief    The default float value.  Meaningless if _required is true.
   */
  float _default;

  /*!
   @brief    Scale value.  Useful for converting units at parse time (i.e. from degrees to radians).
   */
  float _scale;

  /*!
   @brief    The parsed generator.
   */
  FloatGenerator* _generator;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class which specifies an xml attribute with a 2D float distribution value.
 */
class MENGE_API Vec2DDistributionAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   The 2D Vector generator does not accept an optional prefix like the float and int distributions.

   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided. This will be
            interpreted as a const distribution.
   @param    scale      The optional scale value for this float distribution.
   */
  Vec2DDistributionAttribute(bool required, const Vector2& defValue, float scale = 1.f)
      : Attribute("", required), _default(defValue), _scale(scale), _generator(0x0) {}

  /*!
   @brief    Destructor
   */
  virtual ~Vec2DDistributionAttribute();

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _generator = 0x0; }

  /*!
   @brief    Retrieve the parsed 2D float generator.

   @returns    A pointer to a 2D float generator.  The float generator becomes the responsibility of
              the caller.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual Vec2DGenerator* getVec2DGenerator() {
    Vec2DGenerator* val = _generator;
    _generator = 0x0;
    return val;
  }

 protected:
  /*!
   @brief   The default float value.  Meaningless if _required is true.
   */
  Vector2 _default;

  /*!
   @brief   Scale value.  Useful for converting units at parse time (i.e. from
   degrees to radians.
   */
  float _scale;

  /*!
   @brief   The parsed generator.
   */
  Vec2DGenerator* _generator;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief   The class which specifies an xml attribute with an int distribution value.
 */
class MENGE_API IntDistributionAttribute : public Attribute {
 public:
  /*!
   @brief    Constructor.

   @param    name      The attribute name - in this case, interpreted as the the prefix for the
                      distribution values.
   @param    required  Whether or not the attribute is required (true) or not (false).
   @param    defValue  The default value for the attribute if none is provided. This will be
                      interpreted as a const distribution.
   */
  IntDistributionAttribute(const std::string& name, bool required, int defValue)
      : Attribute(name, required), _default(defValue), _generator(0x0) {}

  /*!
   @brief    Destructor
   */
  virtual ~IntDistributionAttribute();

  /*!
   @brief    Extracts the values for this attribute from the xml node.

   This is the main parsing work.  It must be performed by each attribute sub-class.

   @param    node      The XML node containing the attributes.
   @returns  True on sucessful extraction, false otherwise.
   */
  virtual bool extract(TiXmlElement* node);

  /*!
   @brief    If defined, sets the default value for the attribute.
   */
  virtual void setDefault() { _generator = 0x0; }

  /*!
   @brief    Retrieve the parsed int generator.

   @returns    A pointer to a float generator.  The float generator becomes the responsibility of
              the caller.
   @throws    An AttributeDefinitionException if the types are wrong.
   */
  virtual IntGenerator* getIntGenerator() {
    IntGenerator* val = _generator;
    _generator = 0x0;
    return val;
  }

 protected:
  /*!
   @brief    The default int value.  Meaningless if _required is true.
   */
  int _default;

  /*!
   @brief    The parsed generator.
   */
  IntGenerator* _generator;
};
}  // namespace Menge
#endif  // __ATTRIBUTE_H__
