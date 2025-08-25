#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QSharedPointer>
#include <QByteArray>

#include <memory_resource>
#include <stdexcept>

/// @brief Determine if the system is big endianness
/// @return True if it is big endianness
[[deprecated("Outdated")]] consteval bool isBigEndianness();

/// @brief Convert number of endianness
/// @tparam T Type of integral
/// @param value Integral of the other endianness
/// @return Integral of new endianness
template <typename T>
    requires std::integral<T>
[[nodiscard]] constexpr T swapEndianness(T value) {
    T result = 0;
    for (qsizetype i = 0; i < sizeof(value); ++i) {
        result = (result << 8) | ((value >> (8 * i)) & 0xFF);
    }
    return result;
}

/// @brief Convert if the network endianness is different from local system
/// @tparam T Type of integral
/// @param value Integral of the other endianness
/// @return Integral of new endianness
template <typename T>
    requires std::integral<T>
[[nodiscard]] inline T swapNetworkEndianness(T value) {
    if constexpr (std::endian::native == std::endian::little) {
        return swapEndianness(value);
    }
    return value;
}

class DataPackage
{
public:
    using LengthType = quint32;
    using RequestIDType = qint64;
    using DataType = quint8;

    enum DataPackageType : LengthType {
        Unknown = 0,
        Text = 1,
        Binary = 2,
        FileStream = 3,
        HeartBeat = 4
    };
private:
#pragma pack(1)
    LengthType length = 0;          ///< Length of the data package.
public:
    DataPackageType type = Unknown; ///< Type identifier of the data package.
    LengthType sequenceSize = 1;    ///< Sequence size.
    LengthType sequence = 0;        ///< Sequence number of the data package.
    RequestIDType requestID = 0;    ///< Request ID associated with the data package.
private:
    DataType data[0];               ///< Data in the pack
#pragma pack()

public:
    DataPackage() = delete;
    ~DataPackage() noexcept = default;
    DataPackage(const DataPackage &) = delete;
    DataPackage(DataPackage &&) = delete;

    DataPackage &operator=(const DataPackage &) = delete;
    DataPackage &operator=(DataPackage &&) = delete;

    /**
   * @brief Creates a data package from the given data.
   * @return Shared pointer to the created data package.
   */
    [[nodiscard]] static QSharedPointer<DataPackage>
    makePackage(QByteArrayView data,
                DataPackageType type = DataPackageType::Unknown,
                LengthType sequenceSize = 1, LengthType sequence = 0,
                RequestIDType requestID = 0) {
        const qsizetype lenth = sizeof(DataPackage) + data.size();
        void *mem =
            local_datapack_sync_pool.allocate(static_cast<LengthType>(lenth));
        std::memset(mem, 0, lenth);
        QSharedPointer<DataPackage> package(
            static_cast<DataPackage *>(mem), [lenth](DataPackage *data_package) {
                data_package->~DataPackage();
                local_datapack_sync_pool.deallocate(data_package, lenth);
            });
        package->length = static_cast<LengthType>(lenth);
        std::memcpy(package->data, data.data(), data.size());
        package->type = type;
        package->sequenceSize = sequenceSize;
        package->sequence = sequence;
        package->requestID = requestID;
        return package;
    }

    /**
   * @brief Loads a data package from binary data.
   * @param data Binary data representing a data package.
   * @return Shared pointer to the loaded data package.
   */
    [[nodiscard]] static QSharedPointer<DataPackage>
    stringToPackage(QByteArrayView data) {
        // Check if the package data is too small
        if (data.size() < sizeof(DataPackage)) {
            throw std::logic_error("data is too small");
        }

        // Data package length
        LengthType size = 0;
        std::memcpy(&size, data.data(), sizeof(LengthType));
        if (std::endian::native == std::endian::little) {
            size = swapEndianness(size);
        }

        // Error handling if data package length does not match actual size,
        // if length is smaller than the default package size
        if (size != data.size() || size < sizeof(DataPackage)) {
            throw std::logic_error("invalid data");
        }

        // Allocate memory and construct the DataPackage
        void *mem = local_datapack_sync_pool.allocate(size);
        std::memset(mem, 0, size);
        QSharedPointer<DataPackage> package(
            static_cast<DataPackage *>(mem),
            [lenth = size](DataPackage *data_package) {
                data_package->~DataPackage();
                local_datapack_sync_pool.deallocate(data_package, lenth);
            });
        // Copy the data from string
        std::memcpy(package.get(), data.data(), size);

        // Process data in package
        if constexpr (std::endian::native == std::endian::little) {
            // Endianness conversion
            package->length = swapEndianness(package->length);
            package->type = static_cast<DataPackageType>(
                swapEndianness(static_cast<LengthType>(package->type)));
            package->sequenceSize = swapEndianness(package->sequenceSize);
            package->sequence = swapEndianness(package->sequence);
            package->requestID = swapEndianness(package->requestID);
        }

        return package;
    }

    /**
   * @brief Converts this data package to a binary string.
   * @return Binary data representing this data package.
   */
    [[nodiscard]] QByteArray packageToString() const {
        QByteArray strdata;
        strdata.resize(this->length);
        // Copy this memory data into strdata
        std::memcpy(strdata.data(), this, this->length);
        // Converse the string pointer to DataPackage pointor to process data
        DataPackage *package = reinterpret_cast<DataPackage *>(strdata.data());

        // Process string data
        if constexpr (std::endian::native == std::endian::little) {
            // Endianness conversion
            package->length = swapEndianness(package->length);
            package->type = static_cast<DataPackageType>(
                swapEndianness(static_cast<LengthType>(package->type)));
            package->sequenceSize = swapEndianness(package->sequenceSize);
            package->sequence = swapEndianness(package->sequence);
            package->requestID = swapEndianness(package->requestID);
        }

        return strdata;
    }

    /**
   * @brief Gets the size of this data package.
   * @return Size of this data package.
   */
    [[nodiscard]] qsizetype getPackageSize() const noexcept {
        return static_cast<qsizetype>(this->length);
    }

    /**
   * @brief Gets the size of the original data in this data package.
   * @return Size of the original data in this data package.
   */
    [[nodiscard]] qsizetype getDataSize() const noexcept {
        return static_cast<qsizetype>(this->length) - sizeof(DataPackage);
    }

    /**
   * @brief Gets the original data in this data package.
   * @return Original data in this data package.
   */
    [[nodiscard]] QByteArray getData() const {
        return {reinterpret_cast<const char *>(this->data), this->getDataSize()};
    }

    /**
   * @brief Retrieves the original data by populating the provided buffer.
   * @param[out] buffer The target string to store the data.
   *                    - Existing contents will be cleared and overwritten.
   *                    - For optimal performance with large data, pre-allocate
   * capacity via `buffer.reserve()` to avoid reallocations.
   * @throw std::bad_alloc If memory allocation fails during buffer resize.
   */
    void getData(std::string &buffer) const {
        buffer.assign(reinterpret_cast<const char *>(this->data),
                      this->getDataSize());
    }

    /**
   * @brief Retrieves the original data by populating the provided buffer.
   * @param[out] buffer The target string to store the data.
   *                    - Existing contents will be cleared and overwritten.
   *                    - For optimal performance with large data, pre-allocate
   * capacity via `buffer.reserve()` to avoid reallocations.
   * @throw std::bad_alloc If memory allocation fails during buffer resize.
   */
    void getData(QByteArray &buffer) const {
        buffer.assign({reinterpret_cast<const char *>(this->data),
                       this->getDataSize()});
    }

    /**
   * @brief Retrieves the original data by populating the provided PMR-enabled
   * buffer.
   * @param[out] buffer The target `std::pmr::string` to store the data.
   *                    - Existing contents will be **cleared and overwritten**.
   *                    - Uses the buffer's **associated memory resource**
   * (e.g., memory pool or arena).
   *                    - For optimal performance, pre-allocate capacity via
   * `buffer.reserve()`.
   * @note This overload requires C++17 or later and is intended for advanced
   * memory management scenarios. Ensure the buffer's allocator matches the
   * desired memory strategy (e.g., monotonic or pooled).
   * @throw std::bad_alloc If the buffer's allocator fails to allocate memory.
   */
    void getData(std::pmr::string &buffer) const {
        buffer.assign(reinterpret_cast<const char *>(this->data),
                      this->getDataSize());
    }

private:
    inline static std::pmr::synchronized_pool_resource local_datapack_sync_pool =
        {};
};

#endif // DATAPACKAGE_H
