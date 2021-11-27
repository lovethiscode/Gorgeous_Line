#ifndef QUICKLIST_H
#define QUICKLIST_H
#include <QMap>
#include <QList>
#include <QString>

template<class T>
class QuickList
{
public:
    void append(const T& data) {
        auto iter =  data_.insert(data_.end(), data);
        map_[data.id] = iter;
    }

    void append(T&& data) {
        QString id = data.id;
        auto iter =  data_.insert(data_.end(), std::move(data));
        map_[id] = iter;
    }

    int length() const {
        return data_.length();
    }

    const T &operator[](int i) const {
        return data_[i];
    }
    T &operator[](int i) {
        return data_[i];
    }

    int findIndex(const QString &id, int default_index)
    {
        for(int i=0; i< data_.size(); i++) {
            if(data_.at(i).id == id) {
                return i;
            }
        }

        return default_index;
    }

    void RemoveRow(int row, int count) {
        // 先查找位置
        int current_row = 0;
        auto begin = data_.begin();
        for(; begin != data_.end(); ) {
            if(current_row == row) {
                //找到位置了，开始删除
                auto iter = map_.find(begin->id);
                if(iter != map_.end()) {
                    map_.erase(iter);
                }

                begin = data_.erase(begin);
                row ++;
                //判断是否删除完了
                count--;
                if(count == 0) {
                    break;
                }
            } else {
                begin ++;
            }
            current_row++;
        }
    }


    void earse(const QString& id) {
        auto iter = map_.find(id);
        if(iter != map_.end()) {
            data_.erase(iter->second);
            map_.erase(iter);
        }
    }

private:
    QList<T> data_;
    QMap<QString, typename QList<T>::iterator> map_;
};

#endif // QUICKLIST_H
