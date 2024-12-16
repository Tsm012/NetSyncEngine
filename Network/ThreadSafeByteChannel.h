#pragma once
#include <condition_variable>
#include <mutex>
#include <optional>
#include <vector>

class ThreadSafeByteChannel
{
public:
	void setDataToSend(std::vector<unsigned char> data)
	{
		std::lock_guard<std::mutex> lock(sendDataMtx);
		dataToSend = data;  // Copy data to the vector
		sendDataReady = true;
		cvSend.notify_one();
	}

	std::optional<std::vector<unsigned char>> fetchSendData()
	{
		std::lock_guard<std::mutex> lock(sendDataMtx);
		if (sendDataReady)
		{
			sendDataReady = false;
			return dataToSend;
		}
		else
		{
			return std::nullopt;
		}
	}

	void setReceivedData(std::vector<unsigned char> data)
	{
		std::lock_guard<std::mutex> lock(receviedDataMtx);
		receivedData = data;  // Copy data to the vector
		receivedDataReady = true;
		cvReceived.notify_one();
	}

	std::optional<std::vector<unsigned char>> fetchReceivedData()
	{
		std::lock_guard<std::mutex> lock(receviedDataMtx);
		if (receivedDataReady)
		{
			receivedDataReady = false;
			return receivedData;
		}
		else
		{
			return std::nullopt;
		}
	}

private:
	std::vector<unsigned char> receivedData;
	bool receivedDataReady = false;
	std::mutex receviedDataMtx;

	std::vector<unsigned char> dataToSend;
	bool sendDataReady = false;
	std::mutex sendDataMtx;

	std::condition_variable cvReceived;
	std::condition_variable cvSend;
};
