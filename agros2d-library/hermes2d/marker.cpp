// This file is part of Agros2D.
//
// Agros2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Agros2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Agros2D.  If not, see <http://www.gnu.org/licenses/>.
//
// hp-FEM group (http://hpfem.org/)
// University of Nevada, Reno (UNR) and University of West Bohemia, Pilsen
// Email: agros2d@googlegroups.com, home page: http://hpfem.org/agros2d/

#include "marker.h"
#include "module.h"
#include "scene.h"
#include "util.h"
#include "hermes2d/field.h"
#include "hermes2d/problem.h"

Marker::Marker(FieldInfo *fieldInfo, QString name)
    : m_fieldInfo(fieldInfo), m_name(name), m_isNone(false)
{
}

Marker::~Marker()
{
    m_values.clear();
}

Value &Marker::value(const QString &id)
{
    if (!id.isEmpty())
        return m_values[id];

    assert(0);
}

const QMap<QString, Value> Marker::values() const
{
    return m_values;
}

bool Marker::evaluate(const QString &id, double time)
{
    return m_values[id].evaluateAtTime(time);
}

bool Marker::evaluateAllVariables()
{
    foreach (QString key, m_values.keys())
        if (!evaluate(key, 0.0))
            return false;

    return true;
}

QString Marker::fieldId()
{
    return m_fieldInfo->fieldId();
}

Boundary::Boundary(FieldInfo *fieldInfo, QString name, QString type,
                   QMap<QString, Value> values) : Marker(fieldInfo, name)
{
    // type
    setType(type);

    // set values
    this->m_values = values;
    if (!isNone() && !m_type.isEmpty())
    {
        if (this->m_values.isEmpty())
        {
            Module::BoundaryType boundaryType = fieldInfo->boundaryType(type);
            foreach (Module::BoundaryTypeVariable variable, boundaryType.variables())
            {
                // default for GUI
                Module::DialogRow row = fieldInfo->boundaryUI().dialogRow(variable.id());
                this->m_values[variable.id()] = Value(QString::number(row.defaultValue()));
            }
        }
    }
}

Material::Material(FieldInfo *fieldInfo, QString name,
                   QMap<QString, Value> values) : Marker(fieldInfo, name)
{
    // name and type
    this->m_values = values;

    // set values
    if (name != "none")
    {
        if (this->m_values.isEmpty())
        {
            foreach (Module::MaterialTypeVariable variable, fieldInfo->materialTypeVariables())
            {
                // default for GUI
                Module::DialogRow row = fieldInfo->materialUI().dialogRow(variable.id());
                this->m_values[variable.id()] = Value(QString::number(row.defaultValue()));
            }
        }
    }
}
