// cpp_sanity.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <was/storage_account.h>
#include <was/blob.h>
#include <was/table.h>
#include <cpprest/filestream.h>  
#include <cpprest/containerstream.h> 

std::wstring random_string(size_t length)
{
    auto randchar = []() -> char
    {
        srand(time(NULL));
        const char charset[] =
            "0123456789"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::wstring str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

int main()
{
    const utility::string_t storage_connection_string(U("DefaultEndpointsProtocol=https;AccountName=tankstdcppt;AccountKey=3D16FN4E/kHaFTD/qoK7+kPgY1XxaEiPaghiNOKVSRL2CnW+J/nuxhECimPG8owJF5mhJbIVHDrCoTZjAji5oQ==;EndpointSuffix=core.windows.net"));

    try
    {
        // Retrieve storage account from connection string.
        azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);

        // Create the blob client.
        azure::storage::cloud_blob_client blob_client = storage_account.create_cloud_blob_client();

        // Retrieve a reference to a container.
        azure::storage::cloud_blob_container container = blob_client.get_container_reference(random_string(10));

        // Create the container if it doesn't already exist.
        container.create_if_not_exists();

        // Create or overwrite the "my-blob-1" and "my-blob-1" blobs with contents from text.
        // Retrieve a reference to a blob named "my-blob-2".
        azure::storage::cloud_block_blob blob1 = container.get_block_blob_reference(U("my-blob-1"));
        blob1.upload_text(U("more text"));

        // Retrieve a reference to a blob named "my-blob-3".
        azure::storage::cloud_block_blob blob2 = container.get_block_blob_reference(U("my-directory/my-sub-directory/my-blob-2"));
        blob2.upload_text(U("other text"));

        assert(blob1.download_text() == U("more text"));
        assert(blob2.download_text() == U("other text"));

        container.delete_container_if_exists();
    }
    catch (const std::exception& e)
    {
        std::wcout << U("Error: ") << e.what() << std::endl;
        throw e;
    }

    return 0;
}

