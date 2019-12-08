-- this trigger was rejected by mysql because it altered the
-- same table that triggered it.
-- Can't update table 'cb_blockchain' in stored function/trigger because it is already used by statement which invoked this stored function/trigger.
-- leaving it here for future reference

DELIMITER //

CREATE TRIGGER trig_cb_blockchain_insert AFTER INSERT ON cb_blockchain
    FOR EACH ROW
BEGIN

    DECLARE last_block_height INT(11);

    -- set the previous block's next-hash to the new block hash
    -- this non-standard optimization makes our chain a double linked list
    UPDATE cb_blockchain
    SET hash_next_block = NEW.hash_this_block
    WHERE hash_this_block = NEW.hash_prev_block;

    -- get the chain block height before this block
    SELECT block_height
    INTO last_block_height
    FROM cb_blockchain
    WHERE hash_this_block = NEW.hash_prev_block;

    -- update the new chain block height
    UPDATE cb_blockchain
        SET block_height = last_block_height + 1
    WHERE hash_this_block = NEW.hash_this_block;

END;//

DELIMITER ;