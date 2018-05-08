/******************************************************************************

  This source file is part of the tomviz project.

  Copyright Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "ImageStackModel.h"

#include <QBrush>

namespace tomviz {

ImageStackModel::ImageStackModel(QObject* parent,
                                 QList<ImageInfo>& filesInfo)
  : QAbstractTableModel(parent), m_filesInfo(filesInfo)
{}

int ImageStackModel::rowCount(const QModelIndex&) const
{
  return m_filesInfo.size();
}

int ImageStackModel::columnCount(const QModelIndex&) const
{
  return 4;
}

QVariant ImageStackModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()){
    return QVariant();
  }

  int row = index.row();
  int col = index.column();

  if (row >= m_filesInfo.size()) {
    return QVariant();
  }

  if (role == Qt::DisplayRole){
    if (col == 0) {
      // return m_filesInfo[row].selected ? "Y" : "N";
    } else if (col == 1) {
      return m_filesInfo[row].fileInfo.fileName();
    } else if (col == 2) {
      return QString("%1").arg(m_filesInfo[row].m);
    } else if (col == 3) {
      return QString("%1").arg(m_filesInfo[row].n);
    }
  } else if (role == Qt::ToolTipRole) {
    if (col == 1) {
        return m_filesInfo[row].fileInfo.absoluteFilePath();
    }
  } else if (role == Qt::BackgroundRole) {
    QColor failColor = Qt::red;
    failColor.setAlphaF(0.25);
    QBrush failBackground(failColor);
    QColor successColor = Qt::green;
    if (m_filesInfo[row].selected) {
      successColor.setAlphaF(0.125);
    } else {
      successColor.setAlphaF(0.0625);
    }
    QBrush successBackground(successColor);
    if (m_filesInfo[row].consistent) {
      return successBackground;
    } else {
      return failBackground;
    }
  } else if (role == Qt::CheckStateRole) {
    if (col == 0){
      return m_filesInfo[row].selected;
    }
  }
  return QVariant();
}

QVariant ImageStackModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      if (section == 1) {
        return QString("Filename");
      } else if (section == 2) {
        return QString("X");
      } else if (section == 3) {
        return QString("Y");
      }
    } else if (orientation == Qt::Horizontal) {
      return QString("%1").arg(section + 1);
    }
  }
  return QVariant();
}

Qt::ItemFlags ImageStackModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
      return Qt::ItemIsEnabled;
    }
    if (index.column() == 0) {
      return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
    } else {
      return QAbstractItemModel::flags(index);
    }
}

bool ImageStackModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int col = index.column();
        int row = index.row();
        if (col == 0) {
          m_filesInfo[row].selected = value.toBool();
          emit dataChanged(index, index);
          return true;
        }
    }
    return false;
}

ImageInfo::ImageInfo(QString fileName, int m_, int n_, bool consistent_)
  : fileInfo(QFileInfo(fileName)), m(m_), n(n_), consistent(consistent_)
  , selected(consistent_)
{
}

} // namespace tomviz
