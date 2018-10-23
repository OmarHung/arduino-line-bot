<?php
$type = $_GET['type'];
$channelAccessToken = 'YourChannelAccessToken'; //你的channelAccessToken
$to = 'LineUserId'; //發送給誰
if($to) {
	print_r($to);
	$text = "";
	if($type==0) {
		$text = "系統已啟動";
	}else if($type==1) {
		$text = "警告！有人開門！";
	}
	$message = array(
        'to' => $to,
        'messages' => array(
            array(
                'type' => 'text',
                'text' => $text,
            )
        )
    );
    $curl_post_data = json_encode($message);
    //p($curl_post_data);

    $header = array(
        "Content-Type: application/json",
        'Authorization: Bearer ' . $channelAccessToken,
    );

	$service_url = 'https://api.line.me/v2/bot/message/push';
	$curl = curl_init($service_url);

	//添加HTTP header
	curl_setopt($curl, CURLOPT_HTTPHEADER, $header);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($curl, CURLOPT_POST, true);
	curl_setopt($curl, CURLOPT_POSTFIELDS, $curl_post_data);
	$curl_response = curl_exec($curl);
	print_r($curl_response);
	if ($curl_response === false) {
		$info = curl_getinfo($curl);
		curl_close($curl);
		die('error occured during curl exec. Additioanl info: ' . var_export($info));
	}
	curl_close($curl);
}
?>